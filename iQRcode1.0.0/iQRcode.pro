QT       += core gui multimedia\
            core gui network\
            core gui websockets\
            concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    Cam.cpp \
    Code.cpp \
    CustomLabel.cpp \
    Img_qua_ins.cpp \
    dpage.cpp \
    dpagemenu.cpp \
    main.cpp \
    mainwindow.cpp \
    paddle/clipper.cpp \
    paddle/config.cpp \
    paddle/ocr_cls.cpp \
    paddle/ocr_det.cpp \
    paddle/ocr_rec.cpp \
    paddle/postprocess_op.cpp \
    paddle/preprocess_op.cpp \
    paddle/utility.cpp \
    printer.cpp \
    rotated_object_detection.cpp \
    webapi.cpp \
    Zint/code39.cpp \
    Zint/code128.cpp \
    websocketserver.cpp



HEADERS += \
    customlabel.h \
    dpage.h \
    mainwindow.h \
    mmdeploy/archive/json_archive.h \
    mmdeploy/archive/value_archive.h \
    mmdeploy/classifier.h \
    mmdeploy/classifier.hpp \
    mmdeploy/common.h \
    mmdeploy/common.hpp \
    mmdeploy/core/archive.h \
    mmdeploy/core/device.h \
    mmdeploy/core/device_impl.h \
    mmdeploy/core/graph.h \
    mmdeploy/core/logger.h \
    mmdeploy/core/macro.h \
    mmdeploy/core/mat.h \
    mmdeploy/core/model.h \
    mmdeploy/core/model_impl.h \
    mmdeploy/core/module.h \
    mmdeploy/core/mpl/detected.h \
    mmdeploy/core/mpl/iterator.h \
    mmdeploy/core/mpl/priority_tag.h \
    mmdeploy/core/mpl/span.h \
    mmdeploy/core/mpl/static_any.h \
    mmdeploy/core/mpl/structure.h \
    mmdeploy/core/mpl/type_traits.h \
    mmdeploy/core/net.h \
    mmdeploy/core/operator.h \
    mmdeploy/core/profiler.h \
    mmdeploy/core/registry.h \
    mmdeploy/core/serialization.h \
    mmdeploy/core/status_code.h \
    mmdeploy/core/tensor.h \
    mmdeploy/core/types.h \
    mmdeploy/core/utils/device_utils.h \
    mmdeploy/core/utils/filesystem.h \
    mmdeploy/core/utils/formatter.h \
    mmdeploy/core/utils/source_location.h \
    mmdeploy/core/utils/stacktrace.h \
    mmdeploy/core/value.h \
    mmdeploy/detector.h \
    mmdeploy/detector.hpp \
    mmdeploy/executor.h \
    mmdeploy/experimental/module_adapter.h \
    mmdeploy/model.h \
    mmdeploy/pipeline.h \
    mmdeploy/pipeline.hpp \
    mmdeploy/pose_detector.h \
    mmdeploy/pose_detector.hpp \
    mmdeploy/restorer.h \
    mmdeploy/restorer.hpp \
    mmdeploy/rotated_detector.h \
    mmdeploy/rotated_detector.hpp \
    mmdeploy/segmentor.h \
    mmdeploy/segmentor.hpp \
    mmdeploy/text_detector.h \
    mmdeploy/text_detector.hpp \
    mmdeploy/text_recognizer.h \
    mmdeploy/text_recognizer.hpp \
    mmdeploy/third_party/json/json.hpp \
    mmdeploy/third_party/outcome/outcome-experimental.hpp \
    mmdeploy/third_party/spdlog/async.h \
    mmdeploy/third_party/spdlog/async_logger-inl.h \
    mmdeploy/third_party/spdlog/async_logger.h \
    mmdeploy/third_party/spdlog/cfg/argv.h \
    mmdeploy/third_party/spdlog/cfg/env.h \
    mmdeploy/third_party/spdlog/cfg/helpers-inl.h \
    mmdeploy/third_party/spdlog/cfg/helpers.h \
    mmdeploy/third_party/spdlog/common-inl.h \
    mmdeploy/third_party/spdlog/common.h \
    mmdeploy/third_party/spdlog/details/backtracer-inl.h \
    mmdeploy/third_party/spdlog/details/backtracer.h \
    mmdeploy/third_party/spdlog/details/circular_q.h \
    mmdeploy/third_party/spdlog/details/console_globals.h \
    mmdeploy/third_party/spdlog/details/file_helper-inl.h \
    mmdeploy/third_party/spdlog/details/file_helper.h \
    mmdeploy/third_party/spdlog/details/fmt_helper.h \
    mmdeploy/third_party/spdlog/details/log_msg-inl.h \
    mmdeploy/third_party/spdlog/details/log_msg.h \
    mmdeploy/third_party/spdlog/details/log_msg_buffer-inl.h \
    mmdeploy/third_party/spdlog/details/log_msg_buffer.h \
    mmdeploy/third_party/spdlog/details/mpmc_blocking_q.h \
    mmdeploy/third_party/spdlog/details/null_mutex.h \
    mmdeploy/third_party/spdlog/details/os-inl.h \
    mmdeploy/third_party/spdlog/details/os.h \
    mmdeploy/third_party/spdlog/details/periodic_worker-inl.h \
    mmdeploy/third_party/spdlog/details/periodic_worker.h \
    mmdeploy/third_party/spdlog/details/registry-inl.h \
    mmdeploy/third_party/spdlog/details/registry.h \
    mmdeploy/third_party/spdlog/details/synchronous_factory.h \
    mmdeploy/third_party/spdlog/details/tcp_client-windows.h \
    mmdeploy/third_party/spdlog/details/tcp_client.h \
    mmdeploy/third_party/spdlog/details/thread_pool-inl.h \
    mmdeploy/third_party/spdlog/details/thread_pool.h \
    mmdeploy/third_party/spdlog/details/udp_client-windows.h \
    mmdeploy/third_party/spdlog/details/udp_client.h \
    mmdeploy/third_party/spdlog/details/windows_include.h \
    mmdeploy/third_party/spdlog/fmt/bin_to_hex.h \
    mmdeploy/third_party/spdlog/fmt/bundled/args.h \
    mmdeploy/third_party/spdlog/fmt/bundled/chrono.h \
    mmdeploy/third_party/spdlog/fmt/bundled/color.h \
    mmdeploy/third_party/spdlog/fmt/bundled/compile.h \
    mmdeploy/third_party/spdlog/fmt/bundled/core.h \
    mmdeploy/third_party/spdlog/fmt/bundled/format-inl.h \
    mmdeploy/third_party/spdlog/fmt/bundled/format.h \
    mmdeploy/third_party/spdlog/fmt/bundled/locale.h \
    mmdeploy/third_party/spdlog/fmt/bundled/os.h \
    mmdeploy/third_party/spdlog/fmt/bundled/ostream.h \
    mmdeploy/third_party/spdlog/fmt/bundled/printf.h \
    mmdeploy/third_party/spdlog/fmt/bundled/ranges.h \
    mmdeploy/third_party/spdlog/fmt/bundled/xchar.h \
    mmdeploy/third_party/spdlog/fmt/chrono.h \
    mmdeploy/third_party/spdlog/fmt/compile.h \
    mmdeploy/third_party/spdlog/fmt/fmt.h \
    mmdeploy/third_party/spdlog/fmt/ostr.h \
    mmdeploy/third_party/spdlog/fmt/ranges.h \
    mmdeploy/third_party/spdlog/fmt/xchar.h \
    mmdeploy/third_party/spdlog/formatter.h \
    mmdeploy/third_party/spdlog/fwd.h \
    mmdeploy/third_party/spdlog/logger-inl.h \
    mmdeploy/third_party/spdlog/logger.h \
    mmdeploy/third_party/spdlog/pattern_formatter-inl.h \
    mmdeploy/third_party/spdlog/pattern_formatter.h \
    mmdeploy/third_party/spdlog/sinks/android_sink.h \
    mmdeploy/third_party/spdlog/sinks/ansicolor_sink-inl.h \
    mmdeploy/third_party/spdlog/sinks/ansicolor_sink.h \
    mmdeploy/third_party/spdlog/sinks/base_sink-inl.h \
    mmdeploy/third_party/spdlog/sinks/base_sink.h \
    mmdeploy/third_party/spdlog/sinks/basic_file_sink-inl.h \
    mmdeploy/third_party/spdlog/sinks/basic_file_sink.h \
    mmdeploy/third_party/spdlog/sinks/daily_file_sink.h \
    mmdeploy/third_party/spdlog/sinks/dist_sink.h \
    mmdeploy/third_party/spdlog/sinks/dup_filter_sink.h \
    mmdeploy/third_party/spdlog/sinks/hourly_file_sink.h \
    mmdeploy/third_party/spdlog/sinks/mongo_sink.h \
    mmdeploy/third_party/spdlog/sinks/msvc_sink.h \
    mmdeploy/third_party/spdlog/sinks/null_sink.h \
    mmdeploy/third_party/spdlog/sinks/ostream_sink.h \
    mmdeploy/third_party/spdlog/sinks/qt_sinks.h \
    mmdeploy/third_party/spdlog/sinks/ringbuffer_sink.h \
    mmdeploy/third_party/spdlog/sinks/rotating_file_sink-inl.h \
    mmdeploy/third_party/spdlog/sinks/rotating_file_sink.h \
    mmdeploy/third_party/spdlog/sinks/sink-inl.h \
    mmdeploy/third_party/spdlog/sinks/sink.h \
    mmdeploy/third_party/spdlog/sinks/stdout_color_sinks-inl.h \
    mmdeploy/third_party/spdlog/sinks/stdout_color_sinks.h \
    mmdeploy/third_party/spdlog/sinks/stdout_sinks-inl.h \
    mmdeploy/third_party/spdlog/sinks/stdout_sinks.h \
    mmdeploy/third_party/spdlog/sinks/syslog_sink.h \
    mmdeploy/third_party/spdlog/sinks/systemd_sink.h \
    mmdeploy/third_party/spdlog/sinks/tcp_sink.h \
    mmdeploy/third_party/spdlog/sinks/udp_sink.h \
    mmdeploy/third_party/spdlog/sinks/win_eventlog_sink.h \
    mmdeploy/third_party/spdlog/sinks/wincolor_sink-inl.h \
    mmdeploy/third_party/spdlog/sinks/wincolor_sink.h \
    mmdeploy/third_party/spdlog/spdlog-inl.h \
    mmdeploy/third_party/spdlog/spdlog.h \
    mmdeploy/third_party/spdlog/stopwatch.h \
    mmdeploy/third_party/spdlog/tweakme.h \
    mmdeploy/third_party/spdlog/version.h \
    mmdeploy/video_recognizer.h \
    mmdeploy/video_recognizer.hpp \
    paddle/ThreadPool.h \
    paddle/clipper.h \
    paddle/config.h \
    paddle/ocr_cls.h \
    paddle/ocr_det.h \
    paddle/ocr_rec.h \
    paddle/postprocess_op.h \
    paddle/preprocess_op.h \
    paddle/utility.h \
    printer.h \
    Zint/code39.h \
    Zint/code128.h \
    websocketserver.h


FORMS += \
    dpage.ui \
    dpage.ui \
    mainwindow.ui \
    websocketserver.ui
msvc{
    QMAKE_CXXFLAGS += /source-charset:utf-8 /execution-charset:utf-8
}

INCLUDEPATH += ./opencv_3.4.3_build_world/install/include \
               ./paddle_inference/paddle/include \
               ./paddle_inference/third_party/install/cryptopp/include \
               ./paddle_inference/third_party/install/glog/include \
               ./paddle_inference/third_party/install/mkldnn/include \
               ./paddle_inference/third_party/install/mklml/include \
               ./paddle_inference/third_party/install/xxhash/include \
               ./MNN_master/include

LIBS += -L./install/x64/vc15/lib -lopencv_world343
LIBS += -L./third_party/install/glog/lib/glog.lib
LIBS += -L./third_party/install/mkldnn/lib -lmkldnn
LIBS += -L./third_party/install/mklml/lib -llibiomp5md -lmklml
LIBS += -L./third_party/install/protobuf/lib/libprotobuf.lib
LIBS += -L./third_party/install/xxhash/lib/xxhash.lib
LIBS += -L./paddle/lib -lpaddle_inference
LIBS += -L./MNN_master/lib -lMNN
LIBS += -L./mmdeploy/lib -lmmdeploy
LIBS += -L./mmdeploy/lib -lmmdeploy_rotated_detector





# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
RC_FILE = imia.rc
TARGET = iQRcode

DISTFILES += \
    mmdeploy/third_party/spdlog/fmt/bundled/fmt.license.rst

