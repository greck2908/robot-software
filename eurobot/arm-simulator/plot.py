from collections import namedtuple
from typing import List, NamedTuple, Tuple

import matplotlib.pyplot as plt


class Span(NamedTuple):
    origin: Tuple[int, int]
    colspan: int
    rowspan: int


class SubFigure(NamedTuple):
    gridspan: Span
    x: List[float]
    y: List[float]
    xlabel: str = ""
    ylabel: str = ""


def compose_plot(figsize, gridsize, plots: List[SubFigure]):
    fig = plt.figure(figsize=figsize)
    for plot in plots:
        origin, colspan, rowspan = plot.gridspan
        subfig = plt.subplot2grid(gridsize, origin, colspan=colspan, rowspan=rowspan, fig=fig)
        subfig.plot(plot.x, plot.y)
        subfig.set_xlabel(plot.xlabel)
        subfig.set_ylabel(plot.ylabel)
