#pragma once
#include <memory>
#include <map>
#include <functional>
#include <iostream>
#include <cassert>

#include "message.h"

using key_type = size_t;
template<typename... Args>
class factory6 {
public:
	factory6() = delete;
	using CreateMethod = std::function<std::unique_ptr<message_base>(std::tuple<Args...>)>;

	static bool Register(key_type id, CreateMethod creator)
	{
		auto& map = get_map();
		auto it = map.find(id);
		if (it == map.end())
		{
			map[id] = creator;
			std::cout << id << " registered\n";
			return true;
		}
		return false;
	}

	static std::unique_ptr<message_base> produce(key_type id, Args... args)
	{
		auto& map = get_map();
		auto it = map.find(id);
		if (it != map.end())
			return it->second(std::make_tuple(args...));

		return nullptr;
	}

	static std::map<key_type, CreateMethod>& get_map() {
		static std::map<key_type, CreateMethod> map;
		return map;
	}
};

template<typename T, typename... Args>
std::unique_ptr<T> create(std::tuple<Args...> t) {
	return std::apply([](Args... args) {
		return std::make_unique<T>(args...);
	}, t);
}

template<class T, class F, typename... Args>
struct register_invoker
{
	register_invoker()
	{
		F::template Register<T, Args...>();
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////
template<class T, class F, typename... Args>
struct register_helper
{
	static const register_invoker<T, F, Args...> _registrationInvoker;
};

template<class T, class F, typename... Args>
const register_invoker<T, F, Args...> register_helper<T, F, Args...>::_registrationInvoker
= register_invoker<T, F, Args...>();

struct register_with_factory
{
	template<class T, typename... Args>
	static void Register()
	{
		factory6<Args...>::Register(T::id, [](std::tuple<Args...> args) { return create<T>(args); });
	}
};

template <typename T, typename... Args>
struct auto_message_factory : register_helper<T, register_with_factory>
{
	static const key_type id;
};

template <typename T, typename... Args>
const key_type auto_message_factory<T, Args...>::id = reinterpret_cast<key_type>(&register_helper<T, register_with_factory, Args...>::_registrationInvoker);

struct message4 : public message_base, auto_message_factory<message4, int>
{
	message4(int a) :a_(a) {}
	void foo() override {
		std::cout << "message4" << std::endl;
	}

	int a_;
};