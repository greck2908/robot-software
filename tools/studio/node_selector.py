import argparse
import datetime
import sys
import threading
import time

import uavcan
from pyqtgraph.Qt import QtCore, QtGui

from network.UavcanNode import UavcanNode
from network.NodeStatusMonitor import NodeStatusMonitor
from viewers.Selector import SelectorWidget


def parse_args():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("interface", help="Serial port or SocketCAN interface")
    parser.add_argument("--dsdl", "-d", help="DSDL path", required=True)
    parser.add_argument("--node_id", "-n", help="UAVCAN Node ID", default=127)

    return parser.parse_args()

class NodeSelectController:
    def __init__(self, model, viewer):
        self.model = model
        self.viewer = viewer

        self.nodes = []
        self.model.set_on_new_node_callback(self.update_selection)

        self.viewer.set_callback(lambda i: print("Current index", i, "selection changed to", self.nodes[i]))
        self.viewer.show()

    def update_selection(self):
        known_nodes = self.model.known_nodes
        nodes_with_name = {k: v for k, v in known_nodes.items() if 'name' in v.keys()}
        self.nodes = list(nodes_with_name[node]['name'] for node in nodes_with_name)
        self.viewer.set_nodes(self.nodes)

def qt_loop():
    import sys
    if (sys.flags.interactive != 1) or not hasattr(QtCore, 'PYQT_VERSION'):
        QtGui.QApplication.instance().exec_()

def main():
    args = parse_args()
    uavcan.load_dsdl(args.dsdl)

    node = UavcanNode(interface=args.interface, node_id=args.node_id)

    app = QtGui.QApplication(sys.argv)

    controller = NodeSelectController(model=NodeStatusMonitor(node), viewer=SelectorWidget(title='Node Selector'))

    node.spin()
    qt_loop()

if __name__ == '__main__':
    main()
