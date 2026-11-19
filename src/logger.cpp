
#include "logger.hpp"
namespace impl{
void log_sink(
    LogLevel const& level,
    source_loc loc,
    std::string_view formatted
){
    LOGF_LN(
        "{}{}{}{} {}",
        detail::get_indent_chars(),
        level.title(),
        global::line_no ? std::format("->[{}:{}]", loc.filename(),loc.line) : "",
        global::timer ? std::format("@{:.2f}", timer::to_seconds(timer::since_epoch())) : "",
        level.style_body(formatted)
   );
}
} //namespace impl
void LOG_LINE_MARKER(std::string_view label, source_loc loc){
    std::println(
        "{}  {}",
        //"{} : {}",
        style::with_fg(
            style::fg_br_red(), std::format("MARKER @ {}:{}", loc.filename(),loc.line)
        ),
        label
   );
}
void LOG_EXIT(int code, source_loc loc){
    auto style = style::fg_green();
    if (code != EXIT_SUCCESS){
        style = style::fg_red() + style::underline();
    }
    impl::log_sink(
        LogLevel{"EXIT"sv,{.title = style, .body = style}},
        loc,
        std::format("Exiting with error code {}",code)
    );
    if (code!= EXIT_SUCCESS){
        cpptrace::generate_trace(1).print(); 
    }
    std::exit(code);
}
