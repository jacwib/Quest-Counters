#ifndef IL2CPP_UTILS_H
#define IL2CPP_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

// function_ptr_t courtesy of DaNike
template<typename TRet, typename ...TArgs>
// A generic function pointer, which can be called with and set to a `getRealOffset` call
using function_ptr_t = TRet(*)(TArgs...);

namespace il2cpp_utils {
    namespace array_utils {
        static char* il2cpp_array_addr_with_size(Il2CppArray *array, int32_t size, uintptr_t idx)
        {
            return ((char*)array) + kIl2CppSizeOfArray + size * idx;
        }
        #define load_array_elema(arr, idx, size) ((((uint8_t*)(arr)) + kIl2CppSizeOfArray) + ((size) * (idx)))

        #define il2cpp_array_setwithsize(array, elementSize, index, value)  \
            do {    \
                void*__p = (void*) il2cpp_utils::array_utils::il2cpp_array_addr_with_size ((array), elementSize, (index)); \
                memcpy(__p, &(value), elementSize); \
            } while (0)
        #define il2cpp_array_setrefwithsize(array, elementSize, index, value)  \
            do {    \
                void*__p = (void*) il2cpp_utils::array_utils::il2cpp_array_addr_with_size ((array), elementSize, (index)); \
                memcpy(__p, value, elementSize); \
                } while (0)
        #define il2cpp_array_addr(array, type, index) ((type*)(void*) il2cpp_utils::array_utils::il2cpp_array_addr_with_size (array, sizeof (type), index))
        #define il2cpp_array_get(array, type, index) ( *(type*)il2cpp_array_addr ((array), type, (index)) )
        #define il2cpp_array_set(array, type, index, value)    \
            do {    \
                type *__p = (type *) il2cpp_array_addr ((array), type, (index));    \
                *__p = (value); \
            } while (0)
        #define il2cpp_array_setref(array, index, value)  \
            do {    \
                void* *__p = (void* *) il2cpp_array_addr ((array), void*, (index)); \
                /* il2cpp_gc_wbarrier_set_arrayref ((array), __p, (MonoObject*)(value));    */\
                *__p = (value);    \
            } while (0)
    }
    // Init all of the usable il2cpp API, if it has yet to be initialized
    // Maximum length of characters of an exception message - 1
    #define EXCEPTION_MESSAGE_SIZE 4096
    // Returns a legible string from an Il2CppException*
    std::string ExceptionToString(Il2CppException* exp);

    // Returns the first matching class from the given namespace and typeName by searching through all assemblies that are loaded.
    Il2CppClass* GetClassFromName(std::string_view name_space, std::string_view type_name);

    inline Il2CppClass* GetClassOfObject(Il2CppObject* instance, std::string_view whosAsking) {
        il2cpp_functions::Init();
        auto klass = il2cpp_functions::object_get_class(instance);
        if (!klass) {
            log(ERROR, "il2cpp_utils: %s: Could not find object's class!", whosAsking.data());
            return nullptr;
        }
        return klass;
    }

    // Seriously, don't un-const the returned Type
    const Il2CppType* MakeRef(const Il2CppType* type);

    // Generally, it's better to just use class_from_type!
    const Il2CppType* UnRef(const Il2CppType* type);

    // Framework provided by DaNike
    namespace il2cpp_type_check {
        // To fix "no member named 'get' in il2cpp_type_check::il2cpp_arg_type_<Blah>", define il2cpp_arg_type_<Blah>!
        // When the Il2CppType* would depend only on the type of T (not its value), just use DEFINE_IL2CPP_ARG_TYPE!
        template<typename T>
        struct il2cpp_arg_type_ {};

        template<typename T>
        using il2cpp_arg_type = il2cpp_arg_type_<T>;

        #define DEFINE_IL2CPP_DEFAULT_TYPE(type, fieldName) \
        template<> \
        struct il2cpp_utils::il2cpp_type_check::il2cpp_arg_type_<type> { \
            static inline Il2CppType const* get(type arg) { \
                return il2cpp_functions::class_get_type(il2cpp_functions::defaults->fieldName##_class); \
            } \
        }

        #define DEFINE_IL2CPP_ARG_TYPE(type, nameSpace, className) \
        template<> \
        struct il2cpp_utils::il2cpp_type_check::il2cpp_arg_type_<type> { \
            static inline Il2CppType const* get(type arg) { \
                return il2cpp_functions::class_get_type(il2cpp_utils::GetClassFromName(nameSpace, className)); \
            } \
        }

        DEFINE_IL2CPP_DEFAULT_TYPE(int8_t, sbyte);
        DEFINE_IL2CPP_DEFAULT_TYPE(uint8_t, byte);
        DEFINE_IL2CPP_DEFAULT_TYPE(int16_t, int16);  // "short"
        DEFINE_IL2CPP_DEFAULT_TYPE(uint16_t, uint16);  // "ushort"
        DEFINE_IL2CPP_DEFAULT_TYPE(int32_t, int32);  // "int"
        DEFINE_IL2CPP_DEFAULT_TYPE(uint32_t, uint32);  // "uint"
        DEFINE_IL2CPP_DEFAULT_TYPE(int64_t, int64);  // "long"
        DEFINE_IL2CPP_DEFAULT_TYPE(uint64_t, uint64);  // "ulong"

        DEFINE_IL2CPP_DEFAULT_TYPE(float, single);
        DEFINE_IL2CPP_DEFAULT_TYPE(double, double);

        DEFINE_IL2CPP_DEFAULT_TYPE(bool, boolean);
        DEFINE_IL2CPP_DEFAULT_TYPE(Il2CppChar, char);

        DEFINE_IL2CPP_ARG_TYPE(long double, "System", "Decimal");
        DEFINE_IL2CPP_ARG_TYPE(Color, "UnityEngine", "Color");
        DEFINE_IL2CPP_ARG_TYPE(Vector2, "UnityEngine", "Vector2");
        DEFINE_IL2CPP_ARG_TYPE(Vector3, "UnityEngine", "Vector3");
        DEFINE_IL2CPP_ARG_TYPE(Quaternion, "UnityEngine", "Quaternion");
        DEFINE_IL2CPP_ARG_TYPE(Scene, "UnityEngine.SceneManagement", "Scene");

        template<>
        struct il2cpp_arg_type_<Il2CppType*> {
            static inline Il2CppType const* get(Il2CppType* arg) {
                return arg;
            }
        };

        template<>
        struct il2cpp_arg_type_<Il2CppClass*> {
            static inline Il2CppType const* get(Il2CppClass* arg) {
                il2cpp_functions::Init();
                return il2cpp_functions::class_get_type(arg);
            }
        };

        template<>
        struct il2cpp_arg_type_<Il2CppObject*> {
            static inline Il2CppType const* get(Il2CppObject* arg) {
                if (!arg) return nullptr;
                il2cpp_functions::Init();
                auto klass = GetClassOfObject(arg, "il2cpp_arg_type_<Il2CppObject*>");
                return il2cpp_functions::class_get_type(klass);
            }
        };

        // Unlike many Il2CppObject*, this has no real subclasses to worry about
        DEFINE_IL2CPP_DEFAULT_TYPE(Il2CppString*, string);

        DEFINE_MEMBER_CHECKER(obj)
        DEFINE_MEMBER_CHECKER(object)
        template<typename T>
        struct il2cpp_arg_type_<T*> {
            static inline Il2CppType const* get(T* arg) {
                // These first 2 conditions handle Il2CppObject subclasses that were created
                //   in libil2cpp via composition instead of inheritance
                auto constexpr hasObj = has_obj<T, Il2CppObject>::value;
                auto constexpr hasObject = has_object<T, Il2CppObject>::value;
                // Double check inheritance here
                if constexpr(std::is_convertible<T*, Il2CppObject*>::value) {
                    return il2cpp_arg_type_<Il2CppObject*>::get(arg);
                } else if constexpr(hasObj) {
                    return il2cpp_arg_type_<Il2CppObject*>::get(&arg->obj);
                } else if constexpr(hasObject) {
                    return il2cpp_arg_type_<Il2CppObject*>::get(&arg->object);
                } else {
                    static_assert(false_t<T*>, "Turning this kind of pointer into an Il2CppType is not implemented! "
                        "Please pass primitives and structs as themselves instead of taking their address. "
                        "If the pointer should be treatable as Il2CppObject*, please file an issue on sc2ad/beatsaber-hook.");
                }
            }
        };
        #undef has_obj
        #undef has_object

        template<typename T>
        struct il2cpp_arg_type_<T&> {
            static inline Il2CppType const* get(T& arg) {
                // A method can store a result back to a non-const ref! Make the type byref!
                auto base = il2cpp_arg_type_<T>::get(arg);
                return MakeRef(base);
            }
        };

        template<typename T>
        struct il2cpp_arg_type_<const T&> {
            static inline Il2CppType const* get(const T& arg) {
                // A method cannot store a result back to a const ref. It is not a C# ref.
                return il2cpp_arg_type_<T>::get(arg);
            }
        };

        template<typename T>
        struct il2cpp_arg_ptr {
            static inline void* get(T const& arg) {
                return reinterpret_cast<void*>(&arg);
            }
        };
        template<typename T>
        struct il2cpp_arg_ptr<T*> {
            static inline void* get(T* arg) {
                return reinterpret_cast<void*>(arg);
            }
        };
    }

    std::vector<const Il2CppType*> ClassVecToTypes(std::vector<const Il2CppClass*> seq);

    // Gets the System.Type Il2CppObject* (actually an Il2CppReflectionType*) for an Il2CppClass*
    Il2CppObject* GetSystemType(const Il2CppClass* klass);

    // Gets the System.Type Il2CppObject* (actually an Il2CppReflectionType*) for the class with the given namespace and name
    Il2CppObject* GetSystemType(std::string_view nameSpace, std::string_view className);

    // Gets the standard class name of an Il2CppClass*
    std::string ClassStandardName(Il2CppClass* klass, bool generics = true);

    // Gets a C# name of a type
    const char* TypeGetSimpleName(const Il2CppType* type);

    bool IsInterface(const Il2CppClass* klass);

    inline auto ExtractTypes() {
        return std::vector<const Il2CppType*>();
    }

    template<typename T>
    std::vector<const Il2CppType*> ExtractTypes(T&& arg) {
        std::vector<const Il2CppType*> type;
        auto typ = il2cpp_type_check::il2cpp_arg_type<T>::get(arg);
        if (typ) {
            type.push_back(typ);
        } else {
            log(ERROR, "ExtractTypes: failed to determine type! Tips: instead of nullptr, pass the Il2CppType* or Il2CppClass* of the argument instead!");
        }
        return type;
    }

    template<typename T, typename... TArgs>
    std::vector<const Il2CppType*> ExtractTypes(T&& arg, TArgs&&... args) {
        auto base = ExtractTypes(arg);
        auto rec = ExtractTypes(args...);
        base.insert(base.end(), rec.begin(), rec.end());
        return base;
    }

    // Returns if a given MethodInfo's parameters match the Il2CppType array provided as type_arr
    bool ParameterMatch(const MethodInfo* method, const Il2CppType* const* type_arr, decltype(MethodInfo::parameters_count) count);

    template<typename... TArgs>
    // Returns if a given MethodInfo's parameters match the Il2CppTypes provided as args
    bool ParameterMatch(const MethodInfo* method, TArgs* ...args) {
        constexpr auto count = sizeof...(TArgs);
        Il2CppType* argarr[] = {reinterpret_cast<Il2CppType*>(args)...};
        return ParameterMatch(method, argarr, count);
    }

    // Returns if a given MethodInfo's parameters match the Il2CppTypes provided as a vector
    inline bool ParameterMatch(const MethodInfo* method, std::vector<const Il2CppType*> seq) {
        return ParameterMatch(method, seq.data(), seq.size());
    }

    // Returns the MethodInfo for the method on the given class with the given name and number of arguments
    // Created by zoller27osu
    const MethodInfo* FindMethodUnsafe(Il2CppClass* klass, std::string_view methodName, int argsCount);

    // Returns the MethodInfo for the method on the given class with the given name and types of arguments
    // Created by zoller27osu
    const MethodInfo* FindMethod(Il2CppClass* klass, std::string_view methodName, std::vector<const Il2CppType*> argTypes);
    const MethodInfo* FindMethod(Il2CppClass* klass, std::string_view methodName, std::vector<const Il2CppClass*> argClasses);
    const MethodInfo* FindMethod(Il2CppClass* klass, std::string_view methodName, std::vector<std::string_view> argSpaceClass);
    // Varargs to vector helper
    template <typename... TArgs, std::enable_if_t<(... && !is_vector<TArgs>::value), int> = 0>  // prevents template recursion
    const MethodInfo* FindMethod(Il2CppClass* klass, std::string_view methodName, TArgs&&... args) {
        if constexpr (sizeof...(TArgs) == 1 && (std::is_integral_v<std::decay_t<TArgs>> && ...)) {
            static_assert(false_t<TArgs...>,
                "FindMethod using argCount is invalid! If argCount is 0 then remove it; otherwise use FindMethodUnsafe!");
        } else if constexpr (sizeof...(TArgs) == 0) {
            return FindMethodUnsafe(klass, methodName, 0);
        } else {
            return FindMethod(klass, methodName, {args...});
        }
    }

    // Returns the MethodInfo for the method on class found via namespace and name with the given other arguments
    template<class... TArgs>
    const MethodInfo* FindMethod(std::string_view nameSpace, std::string_view className, TArgs&&... params) {
        return FindMethod(GetClassFromName(nameSpace, className), params...);
    }
    const MethodInfo* FindMethodUnsafe(std::string_view nameSpace, std::string_view className, std::string_view methodName, int argsCount);

    // Returns the MethodInfo for the method on the given instance
    template<class... TArgs>
    const MethodInfo* FindMethod(Il2CppObject* instance, TArgs&&... params) {
        return FindMethod(GetClassOfObject(instance, "FindMethod"), params...);
    }
    const MethodInfo* FindMethodUnsafe(Il2CppObject* instance, std::string_view methodName, int argsCount);

    inline auto ExtractValues() {
        return std::vector<void*>();
    }

    template<class T>
    std::vector<void*> ExtractValues(T&& arg) {
        using Dt = std::decay_t<T>;
        std::vector<void*> valVec;
        void* val;
        if constexpr (std::is_same_v<Dt, Il2CppType*> || std::is_same_v<Dt, Il2CppClass*>) {
            val = nullptr;
        } else if constexpr (std::is_pointer_v<Dt>) {
            val = reinterpret_cast<void*>(arg);
        } else {
            val = const_cast<Dt*>(&arg);
        }
        valVec.push_back(val);
        return valVec;
    }

    template<class T, class... TArgs>
    std::vector<void*> ExtractValues(T&& arg, TArgs&& ...args) {
        auto base = ExtractValues(arg);
        auto rec = ExtractValues(args...);
        base.insert(base.end(), rec.begin(), rec.end());
        return base;
    }

    template<class TOut, class T, class... TArgs>
    // Runs a MethodInfo with the specified parameters and instance, with return type TOut
    // Assumes a static method if instance == nullptr
    // Returns false if it fails
    // Created by zoller27osu, modified by Sc2ad
    bool RunMethod(TOut* out, T* instance, const MethodInfo* method, TArgs&& ...params) {
        il2cpp_functions::Init();
        if (!method) {
            log(ERROR, "il2cpp_utils: RunMethod: Null MethodInfo!");
            return false;
        }

        // runtime_invoke will assume obj is unboxed and box it. We need to counter that for pre-boxed instances.
        // Note: we could also just call Runtime::Invoke directly, but box non-Il2CppObject instances ourselves as method->klass
        void* inst = instance;
        if constexpr (std::is_same_v<T, Il2CppObject>) {
            if (instance && method->klass->valuetype) {
                // We assume that if the method is for a ValueType and instance is an Il2CppObject, then it was pre-boxed.
                inst = il2cpp_functions::object_unbox(instance);
            }
        }

        Il2CppException* exp = nullptr;
        auto invokeParamsVec = ExtractValues(params...);
        auto ret = il2cpp_functions::runtime_invoke(method, inst, invokeParamsVec.data(), &exp);
        if constexpr (std::is_pointer_v<TOut>) {
            *out = reinterpret_cast<TOut>(ret);
        } else {
            *out = *reinterpret_cast<TOut*>(il2cpp_functions::object_unbox(ret));
        }

        if (exp) {
            log(ERROR, "il2cpp_utils: RunMethod: %s: Failed with exception: %s", il2cpp_functions::method_get_name(method),
                il2cpp_utils::ExceptionToString(exp).c_str());
            return false;
        }
        return true;
    }

    template<class... TArgs, class T>
    // Runs a MethodInfo with the specified parameters and instance; best for void return type
    // Assumes a static method if instance == nullptr
    // Returns false if it fails
    // Created by zoller27osu
    bool RunMethod(T* instance, const MethodInfo* method, TArgs&& ...params) {
        void* out = nullptr;
        return RunMethod(&out, instance, method, params...);
    }

    template<class TOut, class T, class... TArgs>
    // Runs a (static) method with the specified method name, with return type TOut.
    // Checks the types of the parameters against the candidate methods. Returns false if it fails.
    // Created by zoller27osu, modified by Sc2ad
    // TODO: define these via KlassOrInstance types if possible
    std::enable_if_t<std::is_base_of_v<Il2CppClass, T> || std::is_base_of_v<Il2CppObject, T>, bool>
    RunMethod(TOut* out, T* classOrInstance, std::string_view methodName, TArgs&& ...params) {
        il2cpp_functions::Init();
        if (!classOrInstance) {
            log(ERROR, "il2cpp_utils: RunMethod: Null classOrInstance parameter!");
            return false;
        }

        auto types = ExtractTypes(params...);
        if (types.size() != sizeof...(TArgs)) {
            log(WARNING, "RunMethod: ExtractTypes for method %s failed!", methodName.data());
            return false;
        }
        auto method = FindMethod(classOrInstance, methodName, types);
        if (!method) return false;

        if constexpr (std::is_base_of_v<Il2CppClass, T>) {
            return RunMethod<TOut, void>(out, nullptr, method, params...);
        }
        return RunMethod(out, classOrInstance, method, params...);
    }

    template<class TOut, class T, class... TArgs>
    // Runs a (static) method with the specified method name and number of arguments, with return type TOut.
    // DOES NOT PERFORM TYPE CHECKING. Returns false if it fails.
    // Created by zoller27osu, modified by Sc2ad
    std::enable_if_t<std::is_base_of_v<Il2CppClass, T> || std::is_base_of_v<Il2CppObject, T>, bool>
    RunMethodUnsafe(TOut* out, T* classOrInstance, std::string_view methodName, TArgs&& ...params) {
        il2cpp_functions::Init();
        if (!classOrInstance) {
            log(ERROR, "il2cpp_utils: RunMethod: Null classOrInstance parameter!");
            return false;
        }
        auto method = FindMethodUnsafe(classOrInstance, methodName, sizeof...(TArgs));
        if (!method) return false;

        if constexpr (std::is_base_of_v<Il2CppClass, T>) {
            return RunMethod<TOut, void>(out, nullptr, method, params...);
        }
        return RunMethod(out, classOrInstance, method, params...);
    }

    template<class TOut, class... TArgs>
    // Runs a static method with the specified method name and arguments, on the class with the specified namespace and class name.
    // The method also has return type TOut.
    // DOES NOT PERFORM TYPE CHECKING. Returns false if it fails.
    bool RunMethod(TOut* out, std::string_view nameSpace, std::string_view klassName, std::string_view methodName, TArgs&& ...params) {
        auto klass = GetClassFromName(nameSpace, klassName);
        if (!klass) return false;
        return RunMethod(out, klass, methodName, params...);
    }

    template<class TOut, class... TArgs>
    // Runs a static method with the specified method name and arguments, on the class with the specified namespace and class name.
    // The method also has return type TOut.
    // DOES NOT PERFORM TYPE CHECKING. Returns false if it fails.
    bool RunMethodUnsafe(TOut* out, std::string_view nameSpace, std::string_view klassName, std::string_view methodName, TArgs&& ...params) {
        auto klass = GetClassFromName(nameSpace, klassName);
        if (!klass) return false;
        return RunMethodUnsafe(out, klass, methodName, params...);
    }

    template<class... TArgs>
    // Runs a static method with the specified method name and arguments, on the class with the specified namespace and class name.
    // Best for void return type.
    // Checks the types of the parameters against the candidate methods. Returns false if it fails.
    bool RunMethod(std::string_view nameSpace, std::string_view klassName, std::string_view methodName, TArgs&& ...params) {
        void* out = nullptr;
        return RunMethod(&out, nameSpace, klassName, methodName, params...);
    }

    template<class... TArgs>
    // Runs a static method with the specified method name and arguments, on the class with the specified namespace and class name.
    // Best for void return type.
    // DOES NOT PERFORM TYPE CHECKING. Returns false if it fails.
    bool RunMethodUnsafe(std::string_view nameSpace, std::string_view klassName, std::string_view methodName, TArgs&& ...params) {
        void* out = nullptr;
        return RunMethodUnsafe(&out, nameSpace, klassName, methodName, params...);
    }

    template<class T, class... TArgs>
    // Runs a (static) method with the specified method name; best for void return type
    // Checks the types of the parameters against the candidate methods. Returns false if it fails.
    // Created by zoller27osu
    std::enable_if_t<std::is_base_of_v<Il2CppClass, T> || std::is_base_of_v<Il2CppObject, T>, bool>
    RunMethod(T* classOrInstance, std::string_view methodName, TArgs&& ...params) {
        void* out = nullptr;
        return RunMethod(&out, classOrInstance, methodName, params...);
    }

    template<class T, class... TArgs>
    // Runs a (static) method with the specified method name and number of arguments; best for void return type
    // DOES NOT PERFORM TYPE CHECKING. Returns false if it fails.
    // Created by zoller27osu
    std::enable_if_t<std::is_base_of_v<Il2CppClass, T> || std::is_base_of_v<Il2CppObject, T>, bool>
    RunMethodUnsafe(T* classOrInstance, std::string_view methodName, TArgs&& ...params) {
        void* out = nullptr;
        return RunMethodUnsafe(&out, classOrInstance, methodName, params...);
    }

    template<typename TObj = Il2CppObject, typename... TArgs>
    // Creates a new object of the given class using the given constructor parameters and casts it to TObj*
    // Will only run a .ctor whose parameter types match the given arguments.
    TObj* New(Il2CppClass* klass, TArgs const& ...args) {
        il2cpp_functions::Init();

        // object_new call
        auto obj = il2cpp_functions::object_new(klass);
        // runtime_invoke constructor with right type(s) of arguments, return null if constructor errors
        if (!RunMethod(obj, ".ctor", args...)) return nullptr;
        return reinterpret_cast<TObj*>(obj);
    }

    template<typename TObj = Il2CppObject, typename... TArgs>
    // Creates a new object of the given class using the given constructor parameters and casts it to TObj*
    // DOES NOT PERFORM ARGUMENT TYPE CHECKING! Uses the first .ctor with the right number of parameters it sees.
    TObj* NewUnsafe(Il2CppClass* klass, TArgs* ...args) {
        il2cpp_functions::Init();

        // object_new call
        auto obj = il2cpp_functions::object_new(klass);
        // runtime_invoke constructor with right number of args, return null if constructor errors
        if (!RunMethodUnsafe(obj, ".ctor", args...)) return nullptr;
        return reinterpret_cast<TObj*>(obj);
    }

    // Returns the FieldInfo for the field of the given class with the given name
    // Created by zoller27osu
    FieldInfo* FindField(Il2CppClass* klass, std::string_view fieldName);
    // Wrapper for FindField taking a namespace and class name in place of an Il2CppClass*
    template<class... TArgs>
    FieldInfo* FindField(std::string_view nameSpace, std::string_view className, TArgs&&... params) {
        return FindField(GetClassFromName(nameSpace, className), params...);
    }
    // Wrapper for FindField taking an Il2CppObject* in place of an Il2CppClass*
    template<class... TArgs>
    FieldInfo* FindField(Il2CppObject* instance, TArgs&&... params) {
        return FindField(GetClassOfObject(instance, "FindField"), params...);
    }

    // Gets an Il2cppObject* from the given object instance and FieldInfo
    // instance can only be null for static fields
    // Returns nullptr if it fails
    Il2CppObject* GetFieldValue(Il2CppObject* instance, FieldInfo* field);

    // Gets an Il2CppObject* from the given class and field name
    // Returns nullptr if it fails
    // Created by zoller27
    Il2CppObject* GetFieldValue(Il2CppClass* klass, std::string_view fieldName);

    // Gets an Il2CppObject* from the given object instance and field name
    // Returns nullptr if it fails
    // Created by darknight1050, modified by Sc2ad
    Il2CppObject* GetFieldValue(Il2CppObject* instance, std::string_view fieldName);

    // Wrapper around the non-template GetFieldValue's that casts the result for you
    template<class TOut, class TA, class TB>
    std::enable_if_t<std::is_base_of_v<Il2CppObject, TOut>, TOut*>
    GetFieldValue(TA* a, TB b) {
        return reinterpret_cast<TOut*>(GetFieldValue(a, b));
    }

    template<typename TOut>
    // Gets a value from the given object instance, and FieldInfo, with return type TOut
    // Returns false if it fails
    // Assumes a static field if instance == nullptr
    // Created by darknight1050, modified by Sc2ad and zoller27osu
    bool GetFieldValue(TOut* out, Il2CppObject* instance, FieldInfo* field) {
        il2cpp_functions::Init();
        if (!field) {
            log(ERROR, "il2cpp_utils: GetFieldValue: Null FieldInfo!");
            return false;
        }
        if (instance) {
            il2cpp_functions::field_get_value(instance, field, (void*)out);
        } else { // Fallback to perform a static field set
            il2cpp_functions::field_static_get_value(field, (void*)out);
        }
        return true;
    }

    template<typename TOut>
    // Gets the value of the field with type TOut and the given name from the given class
    // Returns false if it fails
    // Adapted by zoller27osu
    bool GetFieldValue(TOut* out, Il2CppClass* klass, std::string_view fieldName) {
        il2cpp_functions::Init();
        if (!klass) {
            log(ERROR, "il2cpp_utils: GetFieldValue: Could not find object class!");
            return false;
        }
        auto field = FindField(klass, fieldName);
        if (!field) return false;
        return GetFieldValue(out, nullptr, field);
    }

    template<typename TOut>
    // Gets a value from the given object instance and field name, with return type TOut
    // Returns false if it fails
    // Created by darknight1050, modified by Sc2ad and zoller27osu
    bool GetFieldValue(TOut* out, Il2CppObject* instance, std::string_view fieldName) {
        il2cpp_functions::Init();
        if (!instance) {
            log(ERROR, "il2cpp_utils: GetFieldValue: Null instance parameter!");
            return false;
        }
        auto field = FindField(instance, fieldName);
        if (!field) return false;
        return GetFieldValue(out, instance, field);
    }

    // An unsafe wrapper around the TOut GetFieldValues; il2cpp should unbox when appropriate
    template<class TOut, class... TArgs>
    TOut GetFieldValueUnsafe(TArgs... params) {
        TOut out;
        GetFieldValue(&out, params...);
        return out;
    }

    // TODO: typecheck the SetField/PropertyValue methods' value params?
    // Sets the value of a given field, given an object instance and FieldInfo
    // Unbox "value" before passing if it is an Il2CppObject but the field is a primitive or struct!
    // Returns false if it fails
    // Assumes static field if instance == nullptr
    bool SetFieldValue(Il2CppObject* instance, FieldInfo* field, void* value);

    // Sets the value of a given field, given an object instance and field name
    // Unbox "value" before passing if it is an Il2CppObject but the field is a primitive or struct!
    // Returns false if it fails
    // Adapted by zoller27osu
    bool SetFieldValue(Il2CppClass* klass, std::string_view fieldName, void* value);

    // Sets the value of a given field, given an object instance and field name
    // Unbox "value" before passing if it is an Il2CppObject but the field is a primitive or struct!
    // Returns false if it fails
    bool SetFieldValue(Il2CppObject* instance, std::string_view fieldName, void* value);

    // Returns the PropertyInfo for the property of the given class with the given name
    // Created by zoller27osu
    const PropertyInfo* FindProperty(Il2CppClass* klass, std::string_view propertyName);
    // Wrapper for FindProperty taking a namespace and class name in place of an Il2CppClass*
    template<class... TArgs>
    const PropertyInfo* FindProperty(std::string_view nameSpace, std::string_view className, TArgs&&... params) {
        return FindProperty(GetClassFromName(nameSpace, className), params...);
    }
    // Wrapper for FindProperty taking an Il2CppObject* in place of an Il2CppClass*
    template<class... TArgs>
    const PropertyInfo* FindProperty(Il2CppObject* instance, TArgs&&... params) {
        return FindProperty(GetClassOfObject(instance, "FindProperty"), params...);
    }

    // Gets an Il2cppObject* from the given object instance and PropertyInfo
    // instance can only be null for static properties
    // Returns nullptr if it fails
    template<class T>
    Il2CppObject* GetPropertyValue(T* instance, const PropertyInfo* prop) {
        il2cpp_functions::Init();
        RET_0_UNLESS(prop);

        auto getter = RET_0_UNLESS(il2cpp_functions::property_get_get_method(prop));
        Il2CppObject* out = nullptr;
        RET_0_UNLESS(RunMethod(out, instance, getter));
        return out;
    }

    // Gets an Il2CppObject* from the given class and property name
    // Returns nullptr if it fails
    Il2CppObject* GetPropertyValue(Il2CppClass* klass, std::string_view propName);

    // Gets an Il2CppObject* from the given object instance and property name
    // Returns nullptr if it fails
    Il2CppObject* GetPropertyValue(Il2CppObject* instance, std::string_view propName);

    // Wrapper around the above GetPropertyValue's that casts the result for you
    template<class TOut, class TA, class TB>
    std::enable_if_t<std::is_base_of_v<Il2CppObject, TOut>, TOut*>
    GetPropertyValue(TA* a, TB b) {
        return reinterpret_cast<TOut*>(GetPropertyValue(a, b));
    }

    template<class TOut, class T>
    // Gets a value from the given object instance, and PropertyInfo, with return type TOut
    // Returns false if it fails
    // Assumes a static property if instance == nullptr
    bool GetPropertyValue(TOut* out, T* instance, const PropertyInfo* prop) {
        il2cpp_functions::Init();
        RET_0_UNLESS(prop);
        auto getter = RET_0_UNLESS(il2cpp_functions::property_get_get_method(prop));
        return RunMethod(out, instance, getter);
    }

    template<typename TOut>
    // Gets the value of the property with type TOut and the given name from the given class
    // Returns false if it fails
    bool GetPropertyValue(TOut* out, Il2CppClass* klass, std::string_view propName) {
        il2cpp_functions::Init();
        RET_0_UNLESS(klass);
        auto prop = RET_0_UNLESS(FindProperty(klass, propName));
        return GetPropertyValue<TOut, void>(out, nullptr, prop);
    }

    template<typename TOut>
    // Gets a value from the given object instance and property name, with return type TOut
    // Returns false if it fails
    bool GetPropertyValue(TOut* out, Il2CppObject* instance, std::string_view propName) {
        il2cpp_functions::Init();
        RET_0_UNLESS(instance);
        auto prop = FindProperty(instance, propName);
        return GetPropertyValue(out, instance, prop);
    }

    // An unsafe wrapper around the TOut GetPropertyValues; il2cpp should unbox when appropriate
    template<class TOut, class... TArgs>
    TOut GetPropertyValueUnsafe(TArgs... params) {
        TOut out;
        GetPropertyValue(&out, params...);
        return out;
    }

    // Sets the value of a given property, given an object instance and PropertyInfo
    // Unbox "value" before passing if it is an Il2CppObject but the property is a primitive or struct!
    // Returns false if it fails
    // Assumes static property if instance == nullptr
    template<class T>
    bool SetPropertyValue(T* instance, const PropertyInfo* prop, void* value) {
        il2cpp_functions::Init();
        RET_0_UNLESS(prop);
        auto setter = RET_0_UNLESS(il2cpp_functions::property_get_set_method(prop));
        return RunMethod(instance, setter, value);
    }

    // Sets the value of a given property, given an object instance and property name
    // Unbox "value" before passing if it is an Il2CppObject but the property is a primitive or struct!
    // Returns false if it fails
    bool SetPropertyValue(Il2CppClass* klass, std::string_view propName, void* value);

    // Sets the value of a given property, given an object instance and property name
    // Unbox "value" before passing if it is an Il2CppObject but the property is a primitive or struct!
    // Returns false if it fails
    bool SetPropertyValue(Il2CppObject* instance, std::string_view propName, void* value);

    template<typename T = MulticastDelegate, typename R, typename... TArgs>
    // Creates an Action and casts it to a MulticastDelegate*
    // actionType should be extracted from the FieldInfo or MethodInfo you plan to send the action to!
    // Created by zoller27osu
    T* MakeAction(Il2CppObject* obj, function_ptr_t<R, TArgs...> callback, const Il2CppType* actionType) {
        Il2CppClass* actionClass = il2cpp_functions::class_from_il2cpp_type(actionType);

        /*
        * TODO: call PlatformInvoke::MarshalFunctionPointerToDelegate directly instead of copying code from it,
        * or at least use a cache like utils::NativeDelegateMethodCache::GetNativeDelegate(nativeFunctionPointer);
        */
        const MethodInfo* invoke = il2cpp_functions::class_get_method_from_name(actionClass, "Invoke", -1);  // well-formed Actions have only 1 invoke method
        MethodInfo* method = (MethodInfo*) calloc(1, sizeof(MethodInfo));
        method->methodPointer = (Il2CppMethodPointer)callback;
        method->invoker_method = NULL;
        method->parameters_count = invoke->parameters_count;
        method->slot = kInvalidIl2CppMethodSlot;
        method->is_marshaled_from_native = true;  // "a fake MethodInfo wrapping a native function pointer"
        // In the event that a function is static, this will behave as normal
        if (obj == nullptr) method->flags |= METHOD_ATTRIBUTE_STATIC;

        // TODO: figure out why passing method directly doesn't work
        auto action = il2cpp_utils::NewUnsafe<T>(actionClass, obj, &method);
        auto asDelegate = reinterpret_cast<Delegate*>(action);
        if (asDelegate->method_ptr != (void*)callback) {
            log(ERROR, "Created Action's method_ptr (%p) is incorrect (should be %p)!", asDelegate->method_ptr, callback);
            return nullptr;
        }
        return action;
    }

    template<typename T = MulticastDelegate>
    T* MakeAction(Il2CppObject* obj, void* callback, const Il2CppType* actionType) {
        auto tmp = reinterpret_cast<function_ptr_t<void>>(callback);
        return MakeAction(obj, tmp, actionType);
    }

    // Calls the System.RuntimeType.MakeGenericType(System.Type gt, System.Type[] types) function
    Il2CppReflectionType* MakeGenericType(Il2CppReflectionType* gt, Il2CppArray* types);

    // Function made by zoller27osu, modified by Sc2ad
    Il2CppClass* MakeGeneric(const Il2CppClass* klass, std::vector<const Il2CppClass*> args);



    // Function made by zoller27osu, modified by Sc2ad
    // Logs information about the given MethodInfo* as log(DEBUG)
    void LogMethod(const MethodInfo* method);

    // Created by zoller27osu
    // Calls LogMethod on all methods in the given class
    void LogMethods(Il2CppClass* klass, bool logParents = false);

    // Created by zoller27osu
    // Logs information about the given FieldInfo* as log(DEBUG)
    void LogField(FieldInfo* field);

    // Created by zoller27osu
    // Calls LogField on all fields in the given class
    void LogFields(Il2CppClass* klass, bool logParents = false);

    // Created by zoller27osu
    // Logs information about the given PropertyInfo* as log(DEBUG)
    void LogProperty(const PropertyInfo* field);

    // Created by zoller27osu
    // Calls LogProperty on all properties in the given class
    void LogProperties(Il2CppClass* klass, bool logParents = false);

    // Some parts provided by zoller27osu
    // Logs information about the given Il2CppClass* as log(DEBUG)
    void LogClass(Il2CppClass* klass, bool logParents = false);

    // Logs all classes (from every namespace) that start with the given prefix
    // WARNING: THIS FUNCTION IS VERY SLOW. ONLY USE THIS FUNCTION ONCE AND WITH A FAIRLY SPECIFIC PREFIX!
    void LogClasses(std::string_view classPrefix, bool logParents = false);

    // Adds the given TypeDefinitionIndex to the class hash table of a given image
    // Mainly used in LogClasses
    void AddTypeToNametoClassHashTable(const Il2CppImage* img, TypeDefinitionIndex index);

    // Adds the given nested types of the namespaze, parentName, and klass to the hastable
    // Mainly used in AddTypeToNametoClassHashTable
    void AddNestedTypesToNametoClassHashTable(Il2CppNameToTypeDefinitionIndexHashTable* hashTable, const char *namespaze, const std::string& parentName, Il2CppClass *klass);

    // Adds the given nested types of typeDefinition to the class hash table of a given image
    // Mainly used in AddTypeToNametoClassHashTable
    void AddNestedTypesToNametoClassHashTable(const Il2CppImage* img, const Il2CppTypeDefinition* typeDefinition);

    // Creates a cs string (allocates it) with the given string_view and returns it
    Il2CppString* createcsstr(std::string_view inp);

    // Returns if a given source object is an object of the given class
    // Created by zoller27osu
    [[nodiscard]] bool Match(const Il2CppObject* source, const Il2CppClass* klass) noexcept;

    // Asserts that a given source object is an object of the given class
    // Created by zoller27osu
    bool AssertMatch(const Il2CppObject* source, const Il2CppClass* klass);

    template<class To, class From>
    // Downcasts a class from From* to To*
    [[nodiscard]] auto down_cast(From* in) noexcept {
        static_assert(std::is_convertible_v<To*, From*>);
        return static_cast<To*>(in);
    }

    template<typename... TArgs>
    // Runtime Invoke, but with a list initializer for args
    Il2CppObject* RuntimeInvoke(const MethodInfo* method, Il2CppObject* reference, Il2CppException** exc, TArgs* ...args) {
        il2cpp_functions::Init();

        void* invokeParams[] = {reinterpret_cast<void*>(args)...};
        return il2cpp_functions::runtime_invoke(method, reference, invokeParams, exc);
    }
}
#endif /* IL2CPP_UTILS_H */
