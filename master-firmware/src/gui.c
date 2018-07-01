#include "gui.h"
#include <ch.h>
#include <hal.h>
#include <string.h>
#include <stdio.h>
#include <gfx.h>
#include "main.h"
#include "commands.h"
#include "gui_utilities2.h"

static GHandle score_label;
static GHandle sensor_label;
static GHandle console;
static GHandle x_label;
static GHandle y_label;
static GHandle a_label;

static bool init_done = false;

#define MSG_MAX_LENGTH 128
#define MSG_BUF_SIZE 16
static char msg_buffer[MSG_MAX_LENGTH][MSG_BUF_SIZE];
static char *msg_mailbox_buf[MSG_BUF_SIZE];
static MAILBOX_DECL(msg_mailbox, msg_mailbox_buf, MSG_BUF_SIZE);
static MEMORYPOOL_DECL(msg_pool, MSG_MAX_LENGTH, NULL);

static void gui_thread(void *p)
{
    (void)p;

    gfxInit();
    gwinSetDefaultStyle(&WhiteWidgetStyle, FALSE);
    gwinSetDefaultFont(gdispOpenFont("DejaVuSans12"));
    gdispClear(Black);
    {
        GWindowInit wi;
        gwinClearInit(&wi);
        wi.show = TRUE;
        wi.x = 0;
        wi.y = 200;
        wi.width = gdispGetWidth();
        wi.height = gdispGetHeight() - 200;
        console = gwinConsoleCreate(0, &wi);
    }
    {
        GWidgetInit wi;
        gwinWidgetClearInit(&wi);
        wi.g.show = TRUE;
        wi.g.x = 0;
        wi.g.y = 0;
        wi.g.width = gdispGetWidth();
        wi.g.height = 40;
        score_label = gwinLabelCreate(0, &wi);
        gwinSetFont(score_label, gdispOpenFont("DejaVuSans32"));
        gwinSetText(score_label, "Score 42", TRUE);
    }
    {
        GWidgetInit wi;
        gwinWidgetClearInit(&wi);
        wi.g.show = TRUE;
        wi.g.x = 0;
        wi.g.y = 45;
        wi.g.width = gdispGetWidth();
        wi.g.height = 40;
        sensor_label = gwinLabelCreate(0, &wi);
        gwinSetFont(sensor_label, gdispOpenFont("DejaVuSans32"));
        gwinSetText(sensor_label, "Hand sensor", TRUE);
    }
    {
        GWidgetInit wi;
        gwinWidgetClearInit(&wi);
        wi.g.show = TRUE;
        wi.g.x = 0;
        wi.g.y = 90;
        wi.g.width = gdispGetWidth();
        wi.g.height = 30;
        x_label = gwinLabelCreate(0, &wi);
        gwinSetFont(x_label, gdispOpenFont("DejaVuSans16"));
        gwinSetText(x_label, "Position x", TRUE);
    }
    {
        GWidgetInit wi;
        gwinWidgetClearInit(&wi);
        wi.g.show = TRUE;
        wi.g.x = 0;
        wi.g.y = 125;
        wi.g.width = gdispGetWidth();
        wi.g.height = 30;
        y_label = gwinLabelCreate(0, &wi);
        gwinSetFont(y_label, gdispOpenFont("DejaVuSans16"));
        gwinSetText(y_label, "Position y", TRUE);
    }
    {
        GWidgetInit wi;
        gwinWidgetClearInit(&wi);
        wi.g.show = TRUE;
        wi.g.x = 0;
        wi.g.y = 160;
        wi.g.width = gdispGetWidth();
        wi.g.height = 30;
        a_label = gwinLabelCreate(0, &wi);
        gwinSetFont(a_label, gdispOpenFont("DejaVuSans16"));
        gwinSetText(a_label, "Position a", TRUE);
    }

    gwinSetColor(console, White);
    gwinSetBgColor(console, Black);
    gwinClear(console);
    chPoolLoadArray(&msg_pool, msg_buffer, MSG_BUF_SIZE);
    init_done = true;

    WARNING("GUI init done");

    chThdSleepMilliseconds(1000);
    while (true)
    {
        char *msg;
        msg_t res = chMBFetch(&msg_mailbox, (msg_t *)&msg, MS2ST(500));

        if (res == MSG_OK)
        {
            gwinPrintf(console, msg);
            chPoolFree(&msg_pool, msg);
        }
        else
        {
            static char buffer[64];
            float hand_sens;
            messagebus_topic_t *hand_distance_topic = messagebus_find_topic(&bus, "/hand_distance");
            if (hand_distance_topic && messagebus_topic_read(hand_distance_topic, &hand_sens, sizeof(hand_sens)))
            {
                if (hand_sens > 0)
                {
                    sprintf(buffer, "Distance: %.0f [mm]", hand_sens * 1000);
                    gwinSetText(sensor_label, buffer, TRUE);
                }
                else
                {
                    sprintf(buffer, "I don't see anything Coco");
                    gwinSetText(sensor_label, buffer, TRUE);
                }
            }
            int score = 0;
            messagebus_topic_t *score_topic = messagebus_find_topic(&bus, "/score");
            if (score_topic && messagebus_topic_read(score_topic, &score, sizeof(score)))
            {
                sprintf(buffer, "Score: %d", score);
                gwinSetText(score_label, buffer, TRUE);
            }
            else
            {
                sprintf(buffer, "Pas de score");
                gwinSetText(score_label, buffer, TRUE);
            }
            if (1)
            {
                robot_position_t pos = get_robot_position();
                sprintf(buffer, "x: %d mm  y: %d mm  a: %d deg", pos.x, pos.y, pos.a);
                gwinSetText(x_label, buffer, TRUE);
                sprintf(buffer, "y: %d mm", pos.y);
                gwinSetText(y_label, buffer, TRUE);
                sprintf(buffer, "a: %d deg", pos.a);
                gwinSetText(a_label, buffer, TRUE);
            }
        }
    }
}

void gui_start()
{
    static THD_WORKING_AREA(wa, 4096);
    chThdCreateStatic(wa, sizeof(wa), LOWPRIO, gui_thread, NULL);
}

void gui_log_console(struct error *e, va_list args)
{
    static char buffer[256];
    if (init_done)
    {
        char *dst = chPoolAlloc(&msg_pool);
        if (dst)
        {
            dst[0] = '\0';
            char color;
            if (e->severity >= ERROR_SEVERITY_WARNING)
            {
                color = '3'; // yellow
            }
            else
            {
                color = 'C'; // no special color
            }
            snprintf(buffer, sizeof(buffer), "\n\033%c%c\033C  \033b%s:%d\033B  ",
                     color, *error_severity_get_name(e->severity),
                     strrchr(e->file, '/') + 1, e->line);
            strcat(dst, buffer);
            vsnprintf(buffer, sizeof(buffer), e->text, args);
            strcat(dst, buffer);
            chMBPost(&msg_mailbox, (msg_t)dst, TIME_IMMEDIATE);
        }
    }
}
