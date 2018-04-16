import numpy as np
import tensorflow as tf
from tensorflow.python.platform import gfile
from tensorflow.python.saved_model import loader
from tensorflow.python.training import saver as saver_lib
from tensorflow.python import pywrap_tensorflow
from tensorflow.python.client import session
from tensorflow.core.framework import graph_pb2
from google.protobuf import text_format
from tensorflow.python.framework import importer

input_checkpoint="/home/long/桌面/resnet_v2_101_2017_04_14/resnet_v2_101.ckpt"
input_graph="/home/long/桌面/resnet_v2_101_2017_04_14/resnet_v2_101_graph.pb"

input_graph_def = graph_pb2.GraphDef()
with gfile.FastGFile(input_graph, "rb") as f:
    input_graph_def.ParseFromString(f.read())

_ = importer.import_graph_def(input_graph_def, name="")


with session.Session() as sess:
    var_list = {}
    reader = pywrap_tensorflow.NewCheckpointReader(input_checkpoint)
    var_to_shape_map = reader.get_variable_to_shape_map()
    for key in var_to_shape_map:
        try:
            tensor = sess.graph.get_tensor_by_name(key + ":0")
        except KeyError:
            continue
        var_list[key] = tensor
    saver = saver_lib.Saver(var_list=var_list)
    saver.restore(sess, input_checkpoint)

for op in tf.get_default_graph().get_operations():
    print(op.name,op.values())

x=tf.get_default_graph().get_tensor_by_name('input:0')
