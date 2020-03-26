#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

// Location of libil2cpp.so
#define IL2CPP_SO_PATH "/data/app/com.beatgames.beatsaber-1/lib/arm64/libil2cpp.so"

#if __has_include(<string_view>)
#include <string_view>
#elif __has_include(<experimental/string_view>)
#include <experimental/string_view>
namespace std {
    using experimental::string_view;
    using experimental::basic_string_view;
    using experimental::u16string_view;
}
#else
#error No string_view implementation available!
#endif
#include <unordered_map>  // breaks if logging is imported before it
#include <thread>

template <typename Container> struct is_vector : std::false_type { };
template <typename... Ts> struct is_vector<std::vector<Ts...> > : std::true_type { };
// TODO: figure out how to write an is_vector_v that compiles properly?

#define MACRO_WRAP(expr) do { \
    expr; \
} while(0)

// Logs error and RETURNS argument 1 IFF argument 2 boolean evaluates as false; else EVALUATES to argument 2
// thank god for this GCC ({}) extension which "evaluates to the last statement"
#define RET_UNLESS(retval, expr) ({ \
    const auto& __temp__ = expr; \
    if (!__temp__) { \
        log(ERROR, "%s (at %s:%i) returned false!", #expr, __FILE__, __LINE__); \
        return retval; \
    } \
    __temp__; \
})
#define RET_V_UNLESS(expr) RET_UNLESS(, expr)
#define RET_0_UNLESS(expr) RET_UNLESS(0, expr)

// Produces a has_[member]<T, U> type trait whose ::value tells you whether T has a member named [member] with type U.
#define DEFINE_MEMBER_CHECKER(member) \
    template<typename T, typename U, typename Enable = void> \
    struct has_ ## member : std::false_type { }; \
    template<typename T, typename U> \
    struct has_ ## member<T, U, \
        typename std::enable_if_t< \
            std::is_same_v<decltype(T::member), U>> \
        > : std::true_type { };

// For use in fire-if-compiled asserts e.g. static_assert(false_t<T>, "message")
template <class...> constexpr std::false_type false_t{};

#include "typedefs.h"
#include "utils-functions.h"
#include "../inline-hook/And64InlineHook.hpp"
#include "logging.h"
#include "il2cpp-functions.hpp"
#include "il2cpp-utils.hpp"  // uses logging.h
#include "../config/config-utils.hpp"

#ifdef __cplusplus

template <typename Function, typename... Args>
static void StartCoroutine(Function&& fun, Args&&... args) {
    auto t = new std::thread(std::forward<Function>(fun), std::forward<Args>(args)...);
    t->detach();
}

// logs the file and line, sleeps to allow logs to flush, then terminates program
void safeAbort(const char* file, int line);
// sets "file" and "line" to the file and line you call this macro from
#define SAFE_ABORT() safeAbort(__FILE__, __LINE__)

template<class T>
auto crashUnless(T&& arg, const char* file, int line) {
    if (!arg) safeAbort(file, line);
    return std::forward<T>(arg);
}
#define CRASH_UNLESS(expr) crashUnless(expr, __FILE__, __LINE__)

extern "C" {
#endif /* __cplusplus */

// Restores an existing stringstream to a newly created state.
void resetSS(std::stringstream& ss);
// Prints the given number of "tabs" as spaces to the given output stream.
void tabs(std::ostream& os, int tabs, int spacesPerTab = 2);
// Logs the given stringstream and clears it.
void print(std::stringstream& ss, LOG_VERBOSE_TYPE lvl = INFO);

// Attempts to print what is stored at the given pointer.
// For a given pointer, it will scan 4 void*'s worth of bytes at the location pointed to.
// For each void* of bytes, it will print the raw bytes and interpretations of the bytes as ints and char*s.
// When the bytes look like a valid pointer, it will attempt to follow that pointer, increasing the indentation.
//   It will not follow pointers that it has already analyzed as a result of the current call.
void analyzeBytes(const void* ptr);

long long getRealOffset(const void* offset);
long long baseAddr(const char* soname);

// Only wildcard is ? and ?? - both are handled the same way. They will skip exactly 1 byte (2 hex digits)
long long findPattern(long long dwAddress, const char* pattern, long long dwSearchRangeLen = 0x1000000);
// Same as findPattern but will continue scanning to make sure your pattern is sufficiently specific.
// Each candidate will be logged. label should describe what you're looking for, like "Class::Init".
// Sets "multiple" iff multiple matches are found, and outputs a log warning message.
// Returns the first match, if any.
long long findUniquePattern(bool& multiple, long long dwAddress, const char* pattern, const char* label = 0, long long dwSearchRangeLen = 0x1000000);

#define MAKE_HOOK(name, addr, retval, ...) \
void* addr_ ## name = (void*) addr; \
retval (*name)(__VA_ARGS__) = NULL; \
retval hook_ ## name(__VA_ARGS__)

#define MAKE_HOOK_OFFSETLESS(name, retval, ...) \
retval (*name)(__VA_ARGS__) = NULL; \
retval hook_ ## name(__VA_ARGS__)

#define MAKE_HOOK_NAT(name, addr, retval, ...) \
void* addr_ ## name = (void*) addr; \
retval (*name)(__VA_ARGS__) = NULL; \
retval hook_ ## name(__VA_ARGS__)

#ifdef __aarch64__

#define INSTALL_HOOK(name) MACRO_WRAP( \
log(INFO, "Installing 64 bit hook: %s", #name); \
A64HookFunction((void*)getRealOffset(addr_ ## name),(void*) hook_ ## name, (void**)&name); \
)

#define INSTALL_HOOK_OFFSETLESS(name, methodInfo) MACRO_WRAP( \
log(INFO, "Installing 64 bit offsetless hook: %s", #name); \
A64HookFunction((void*)methodInfo->methodPointer,(void*) hook_ ## name, (void**)&name); \
)

#define INSTALL_HOOK_NAT(name) MACRO_WRAP( \
log(INFO, "Installing 64 bit native hook: %s", #name); \
A64HookFunction((void*)(addr_ ## name),(void*) hook_ ## name, (void**)&name); \
)

#define INSTALL_HOOK_DIRECT(name, addr) MACRO_WRAP( \
log(INFO, "Installing 64 bit direct hook: %s", #name); \
A64HookFunction((void*)addr, (void*) hook_ ## name, (void**)&name); \
)

// Uninstalls currently just creates a hook at the hooked address
// and sets the hook to call the original function
// No original trampoline is created when uninstalling a hook, hence the nullptr

#define UNINSTALL_HOOK(name) MACRO_WRAP( \
log(INFO, "Uninstalling 64 bit hook: %s", #name); \
A64HookFunction((void*)getRealOffset(addr_ ## name),(void*)&name, (void**)nullptr); \
)

#define UNINSTALL_HOOK_OFFSETLESS(name, methodInfo) MACRO_WRAP( \
log(INFO, "Uninstalling 64 bit offsetless hook: %s", #name); \
A64HookFunction((void*)methodInfo->methodPointer,(void*)&name, (void**)nullptr); \
)

#define UNINSTALL_HOOK_NAT(name) MACRO_WRAP( \
log(INFO, "Uninstalling 64 bit native hook: %s", #name); \
A64HookFunction((void*)(addr_ ## name),(void*)&name, (void**)nullptr); \
)

#define UNINSTALL_HOOK_DIRECT(name, addr) MACRO_WRAP( \
log(INFO, "Uninstalling 64 bit direct hook: %s", #name); \
A64HookFunction((void*)addr, (void*)&name, (void**)nullptr); \
)

#else

#define INSTALL_HOOK(name) \
log(INFO, "Installing 32 bit hook!"); MACRO_WRAP( \
registerInlineHook((uint32_t)getRealOffset(addr_ ## name), (uint32_t)hook_ ## name, (uint32_t **)&name); \
inlineHook((uint32_t)getRealOffset(addr_ ## name)); \
)

#define INSTALL_HOOK_OFFSETLESS(name, methodInfo) MACRO_WRAP( \
log(INFO, "Installing 32 bit offsetless hook!"); \
registerInlineHook((uint32_t)methodInfo->methodPointer, (uint32_t)hook_ ## name, (uint32_t **)&name); \
inlineHook((uint32_t)methodInfo->methodPointer); \
)

#define INSTALL_HOOK_NAT(name) MACRO_WRAP( \
log(INFO, "Installing 32 bit native hook!"); \
registerInlineHook((uint32_t)(addr_ ## name), (uint32_t)hook_ ## name, (uint32_t **)&name); \
inlineHook((uint32_t)(addr_ ## name)); \
)

#define INSTALL_HOOK_DIRECT(name, addr) MACRO_WRAP( \
log(INFO, "Installing 32 bit offsetless hook!"); \
registerInlineHook((uint32_t)addr, (uint32_t)hook_ ## name, (uint32_t **)&name); \
inlineHook((uint32_t)addr); \
)

#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

#include "instruction-parsing.hpp"

#endif /* UTILS_H_INCLUDED */
