#pragma once
#ifndef __cplusplus
	#error "This header is intended for c++ only."
#endif // __cplusplus

#include <type_traits>
#include <print>
#include <cstdlib>
#include <source_location>
#include <string>

#include <libassert/assert.hpp>
#include <cpptrace/basic.hpp>
#include <cpptrace/utils.hpp>

#include "color_utils.hpp"
#include "style.hpp"
#include "util/cppslop.hpp"
#include "util/timer.hpp"
#include "globals.hpp"
#include "source_loc.hpp"

template<typename... Args>
inline constexpr void LOGF_LN(std::format_string<Args...> fmt, Args&&... args){
    std::println(fmt, std::forward<Args>(args)...);
}
struct LogStyle{
    std::string title;
    std::string body;
};

struct LogLevel{
    static constexpr bool use_icons = true;
    std::string_view m_title{"?"};
    LogStyle m_style;
    std::string_view m_icon{"�"};
    auto title() const -> std::string{
        if constexpr (use_icons){
            return style::with(m_style.title, m_icon);
        }else{
            return style::with(m_style.title,m_title);
        }
    }
    auto style_body(std::string_view body_txt) const -> std::string{
        return style::with(m_style.body,body_txt);
    }
};
namespace impl{
void log_sink( LogLevel const& level, source_loc loc, std::string_view body);
} // namespace impl
void LOG_LINE_MARKER(std::string_view label, source_loc loc={}) ;
void LOG_EXIT(int code, source_loc loc = {});

// NOTE: In order to get source location defaulted and also a format string + argument pack after it, we
// need this helper
template <typename... Args>
struct format_string_context__{

    template <class StringLike> requires std::convertible_to<const StringLike&, std::string_view>
    consteval format_string_context__(
        StringLike const& fmt_string, 
        source_loc l = {}
    ) 
        : fmt(fmt_string) , loc(l) {}
    auto format(Args&&... args) -> std::string {
        return std::format(fmt, std::forward<Args>(args)...);
    }
    std::format_string<Args...> fmt;
    source_loc loc;
};

// NOTE: We have to wrap this in type_identity_t so that deduction doesnt try to apply on this argument,
// but rather on the next argument (the variadic one)
template <typename... Args>
using fmt_string_arg = std::type_identity_t<format_string_context__<Args...>>;

namespace log_colors{
    const inline auto fg_DULL_RED   = style::fg_rgb(180, 10, 50);
    const inline auto fg_BRIGHT_RED = style::fg_rgb(255, 10,  0);

    const inline auto fg_GREEN      = style::fg_rgb( 20,170, 30);
    const inline auto fg_BRIGHT_GREEN      = style::fg_rgb( 10,245, 50);
    const inline auto fg_ORANGE     = style::fg_rgb(255, 150, 30);
    const inline auto fg_PURPLE     = style::fg_rgb(255, 90, 255);
    const inline auto fg_GREY       = style::fg_rgb(220,220,230);
    const inline auto fg_WHITE      = style::fg_rgb(255,255,255);
};

template <class... Args>
inline constexpr void LOG_INFO(fmt_string_arg<Args...> ctx, Args&&... a_args) {
    static auto lvl_info = LogLevel{ "INFO"sv, LogStyle{ log_colors::fg_GREY, log_colors::fg_GREY},"ℹ︎"sv };
    impl::log_sink(
        lvl_info,
        ctx.loc,
        ctx.format(std::forward<Args>(a_args)...)
    );
}
template <class... Args>
inline constexpr void LOG_ERROR(fmt_string_arg<Args...> ctx, Args&&... a_args) {
    static auto lvl_error = LogLevel{"ERROR"sv, {log_colors::fg_DULL_RED, log_colors::fg_DULL_RED}, "✖"sv};
    impl::log_sink( lvl_error, ctx.loc, ctx.format(std::forward<Args>(a_args)...));
}


template <class... Args>
inline constexpr void LOG_FATAL(fmt_string_arg<Args...> ctx, Args&&... a_args) {
    static auto lvl_panic = LogLevel{"PANIC"sv, { log_colors::fg_BRIGHT_RED, log_colors::fg_BRIGHT_RED}, "☠︎"sv};
    impl::log_sink( lvl_panic, ctx.loc, ctx.format(std::forward<Args>(a_args)...));
    LOG_EXIT(EXIT_FAILURE);
}
template <class... Args>
inline constexpr void LOG_WARN(fmt_string_arg<Args...> ctx, Args&&... a_args) {
    static auto lvl_warn = LogLevel{"WARN"sv, { log_colors::fg_ORANGE, log_colors::fg_ORANGE}, "⚠︎"sv};
    impl::log_sink( lvl_warn, ctx.loc, ctx.format(std::forward<Args>(a_args)...));

}
#define LOG_DBG_EXPR(expr) LOG_DBG("{} = {}", #expr, expr);
template <class... Args>
inline constexpr void LOG_DBG(fmt_string_arg<Args...> ctx, Args&&... a_args) {
    static auto lvl_dbg = LogLevel{"DBG"sv, { log_colors::fg_PURPLE, log_colors::fg_PURPLE}, "⚙"sv};
    impl::log_sink(
        lvl_dbg,
        ctx.loc,
        ctx.format(std::forward<Args>(a_args)...)
    );
}
namespace detail{
inline u64 indent_level = 0;
inline u64 indent_min= 0;
inline u64 indent_max = 4;
inline constexpr std::string_view indent_string = "  "sv;
inline constexpr auto get_indent_chars() -> std::string{
    auto res = std::string{};
    for (auto i = 0uz; i<indent_level; i++) res+=indent_string;
    return res;
}
} // namespace detail
inline constexpr auto LOG_PUSH_INDENT(u64 diff=1) -> void{
    if (detail::indent_level + diff > detail::indent_max){
        LOG_WARN("Unable to increase indent above max ({}).",detail::indent_max);
    }else{
        detail::indent_level -= diff;
    }
}
inline constexpr auto LOG_POP_INDENT(u64 diff=1) -> void{
    // lvl - diff < min  ==> lvl < min + diff
    if (detail::indent_level < detail::indent_min + diff){
        LOG_WARN("Unable to decrease indent below min ({}).",detail::indent_min);
    }else{
        detail::indent_level -= diff;
    }
}

// NOTE: Clangd doesnt like the repeated use of the THIS_TYPE macro in a single function,
// i dont know why, but it reports 'no matching function call' to stable_type_name on the second
// invocation of the function with a 'decltype(this)' based argument.
// So we just turn them off for clangd
#if CLANGD_PARSE
#define LOG_THIS_CTOR() 
#define LOG_THIS_DTOR() 
#else 
#define LOG_THIS_CTOR() impl::LOG_CTOR__(source_loc{}, stable_type_name<THIS_TYPE>(), cast<void const*>(this))
#define LOG_THIS_DTOR() impl::LOG_DTOR__(source_loc{}, stable_type_name<THIS_TYPE>(), cast<void const*>(this))
#endif 
inline bool mute_ctor_logs = false;
inline bool mute_dtor_logs = false;
namespace impl{
inline constexpr auto LOG_CTOR__(source_loc loc, std::string_view this_type, void const* this_addr) -> void{
    if (mute_ctor_logs){ return;}
    static auto lvl_ctor = LogLevel{"CTOR"sv, { log_colors::fg_BRIGHT_GREEN, log_colors::fg_BRIGHT_GREEN}, "+"sv};
    impl::log_sink(
        lvl_ctor,
        loc,
        std::format("Creating instance of {} ({})", this_type, this_addr)
    );
}
inline constexpr auto LOG_DTOR__(source_loc loc, std::string_view this_type, void const* this_addr) -> void{
    if (mute_dtor_logs){ return;}
    static auto lvl_dtor = LogLevel{"DTOR"sv, { log_colors::fg_BRIGHT_RED, log_colors::fg_BRIGHT_RED}, "-"sv};
    impl::log_sink(
        lvl_dtor,
        loc,
        std::format("Destroying instance of {} ({})", this_type, this_addr)
    );
}
}// namespace impl
