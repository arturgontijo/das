# -*- coding: utf-8 -*-
# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: common.proto
# Protobuf Python Version: 4.25.1
"""Generated protocol buffer code."""

from google.protobuf import descriptor as _descriptor
from google.protobuf import descriptor_pool as _descriptor_pool
from google.protobuf import symbol_database as _symbol_database
from google.protobuf.internal import builder as _builder

# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()


DESCRIPTOR = _descriptor_pool.Default().AddSerializedFile(
    b'\n\x0c\x63ommon.proto\x12\x03\x64\x61s"\x07\n\x05\x45mpty"!\n\x03\x41\x63k\x12\r\n\x05\x65rror\x18\x01 \x01(\x08\x12\x0b\n\x03msg\x18\x02 \x01(\t"!\n\nHandleList\x12\x13\n\x0bhandle_list\x18\x01 \x03(\t"z\n\x0bHandleCount\x12\x37\n\x0chandle_count\x18\x01 \x03(\x0b\x32!.das.HandleCount.HandleCountEntry\x1a\x32\n\x10HandleCountEntry\x12\x0b\n\x03key\x18\x01 \x01(\t\x12\r\n\x05value\x18\x02 \x01(\r:\x02\x38\x01\x62\x06proto3'
)

_globals = globals()
_builder.BuildMessageAndEnumDescriptors(DESCRIPTOR, _globals)
_builder.BuildTopDescriptorsAndMessages(DESCRIPTOR, "common_pb2", _globals)
if _descriptor._USE_C_DESCRIPTORS == False:
    DESCRIPTOR._options = None
    _globals["_HANDLECOUNT_HANDLECOUNTENTRY"]._options = None
    _globals["_HANDLECOUNT_HANDLECOUNTENTRY"]._serialized_options = b"8\001"
    _globals["_EMPTY"]._serialized_start = 21
    _globals["_EMPTY"]._serialized_end = 28
    _globals["_ACK"]._serialized_start = 30
    _globals["_ACK"]._serialized_end = 63
    _globals["_HANDLELIST"]._serialized_start = 65
    _globals["_HANDLELIST"]._serialized_end = 98
    _globals["_HANDLECOUNT"]._serialized_start = 100
    _globals["_HANDLECOUNT"]._serialized_end = 222
    _globals["_HANDLECOUNT_HANDLECOUNTENTRY"]._serialized_start = 172
    _globals["_HANDLECOUNT_HANDLECOUNTENTRY"]._serialized_end = 222
# @@protoc_insertion_point(module_scope)
