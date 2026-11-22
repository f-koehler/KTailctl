ecm_qt_declare_logging_category(
  ktailctl
  HEADER
  "logging_taildrop.hpp"
  IDENTIFIER
  "Logging::Taildrop"
  CATEGORY_NAME
  "org.fkoehler.KTailctl.Taildrop"
  DEFAULT_SEVERITY
  Info)

target_sources(ktailctl PRIVATE taildrop_send_job.hpp taildrop_send_job.cpp taildrop_sender.hpp taildrop_sender.cpp
                                ${CMAKE_CURRENT_BINARY_DIR}/logging_taildrop.cpp)
target_include_directories(ktailctl PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(ktailctl PRIVATE KF6::ThreadWeaver KF6::Notifications)
