from ctypes import *
from collections import namedtuple
import os
from math import *
from operator import itemgetter

import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

liboa = cdll.LoadLibrary(os.path.join(os.path.dirname(__file__), "build", "libobstacle_avoidance.so"))
OA_MAX_POLY = 20

class point_t(Structure):
    _fields_ = [
        ("x", c_float),
        ("y", c_float),
    ]
class poly_t(Structure):
    _fields_ = [
        ("pts", POINTER(point_t)),
        ("l", c_uint)
    ]
class circle_t(Structure):
    _fields_ = [
        ("x", c_float),
        ("y", c_float),
        ("r", c_float),
    ]
path_t = point_t * 50

Circle = namedtuple("Circle", ["x", "y", "radius"])

def initialize_obstacle_avoidance(bbox):
    liboa.polygon_set_boundingbox(c_int(bbox[0]), c_int(bbox[1]), c_int(bbox[2]), c_int(bbox[3]))
    liboa.oa_init()
    liboa.oa_reset()

def add_obstacle(circle, samples, offset=0):
    res = liboa.oa_add_poly_obstacle(circle_t(circle.x, circle.y, circle.radius), c_int(samples), c_float(offset))
    if not res:
        raise("Unable to set obstacle at {} {} with radius {} and {} vertices".format(circle.x, circle.y, circle.radius, samples))

def add_obstacle_from_points(pts):
    obstacle = poly_t()
    liboa.oa_new_poly_(len(pts), pointer(obstacle))
    for i, point in enumerate(pts):
        obstacle.pts[i] = point_t(*point)

def add_cubes_obstacle(x, y, robot_size):
    add_obstacle(Circle(x=x, y=y, radius=90+robot_size/2), 8, pi/8)

def plan(start, end):
    liboa.oa_start_end_points(c_int(start[0]), c_int(start[1]), c_int(end[0]), c_int(end[1]))
    res = liboa.oa_process()
    if res < 0: return []

    path = path_t()
    res_len = liboa.oa_get_path(pointer(path))

    points = [[start[0], start[1]]]
    for i in range(res_len):
        point = point_t()
        liboa.oa_get_point(pointer(point), i)
        points.append([point.x, point.y])

    return points

class Plot():
    def __init__(self):
        self.fig, self.ax = plt.subplots()
        self.ax.set_xlim((0, 3000))
        self.ax.set_ylim((0, 2000))

    def plot_table(self, bbox):
        self.ax.plot([bbox[0], bbox[0], bbox[2], bbox[2], bbox[0]],
                     [bbox[1], bbox[3], bbox[3], bbox[1], bbox[1]], color='black')

        obstacle = poly_t()
        for i in range(OA_MAX_POLY):
            liboa.oa_get_poly(c_int(i), pointer(obstacle))
            if not bool(obstacle.l): break

            polygon = [(obstacle.pts[j].x, obstacle.pts[j].y) for j in range(obstacle.l)]
            self.ax.fill(list(map(itemgetter(0), polygon)), list(map(itemgetter(1), polygon)), "o")

    def plot_path(self, path, start, goal):
        self.ax.plot(list(map(itemgetter(0), path)), list(map(itemgetter(1), path)), marker='o')
        self.ax.plot([start[0]], [start[1]], marker='o', color='red')
        self.ax.plot([goal[0]], [goal[1]], marker='o', color='green')

    def plot_ray(self, start, end, color='black'):
        self.ax.plot([start[0], end[0]], [start[1], end[1]], color=color)

    def save(self, filename):
        self.fig.savefig(filename, dpi=300)
        plt.close(self.fig)

def read_rays():
    rays = []

    for i in range(liboa.oa_get_num_rays() / 4):
        start = point_t()
        end = point_t()
        w = c_int()
        liboa.oa_get_ray(c_int(i), pointer(start), pointer(end), pointer(w))
        rays.append([[start.x, start.y], [end.x, end.y], int(w.value)])

    return rays

if __name__ == '__main__':
    robot_size = 250
    bbox = [int(robot_size/2), int(robot_size/2), int(3000-robot_size/2), int(2000-robot_size/2)]
    initialize_obstacle_avoidance(bbox)

    liboa.mymap_init(c_int(robot_size))

    path = plan((200, 200), (150, 1850))
    path = plan((200, 200), (1010, 700))
    path = plan((1010, 700), (200, 200))

    print('Found path with {} points'.format(len(path)))
    for point in path:
        print('Point {} {}'.format(point[0], point[1]))


    plot = Plot()
    plot.plot_table(bbox)
    plot.plot_path(path, path[0], path[-1])
    for i, ray in enumerate(read_rays()):
        plot.plot_ray(ray[0], ray[1])
    plot.save('test.png')
