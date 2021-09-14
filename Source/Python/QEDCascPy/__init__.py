from .QEDCascPy import RunManager, ThreeVector
import os
path = os.path.dirname(os.path.realpath(__file__))
os.environ["QED_TABLES_PATH"] = path + "/tables"