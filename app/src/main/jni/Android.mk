#
# Copyright 2009 Cedric Priscal
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

LOCAL_PATH := $(call my-dir)


#声明一个EpcCodec so库
include $(CLEAR_VARS)
LOCAL_MODULE := JniDataTransfer
LOCAL_SRC_FILES =: com_sjl_jnidata_JniDataTransfer.cpp

LOCAL_C_INCLUDES := $(NDK_ROOT)/sources/cxx-stl/stlport/stlport
LOCAL_LDLIBS := $(NDK_ROOT)/sources/cxx-stl/stlport/libs/armeabi/libstlport_static.a
#日志注意Android.mk里有一行include $(CLEAR_VARS)必须把LOCAL_LDLIBS :=-llog放在它后面才有用，否则相当于没写
#多个库添加使用+=
LOCAL_LDLIBS    += -lm -llog
#使C++支持异常处理
LOCAL_CPPFLAGS := -fexceptions


include $(BUILD_SHARED_LIBRARY)

