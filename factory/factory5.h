#pragma once
#include <memory>
#include <map>
#include <functional>
#include <iostream>
#include <cassert>

#include "message.h"

namespace factory {
	using key_type = size_t;

	class factory5 {
	public:
		factory5() = delete;
		using CreateMethod = std::function<std::unique_ptr<message_base>()>;

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

		static std::unique_ptr<message_base> produce(key_type id)
		{
			auto& map = get_map();
			auto it = map.find(id);
			if (it != map.end())
				return it->second();

			return nullptr;
		}

		static std::map<key_type, CreateMethod>& get_map() {
			static std::map<key_type, CreateMethod> map;
			return map;
		}
	};

	template<class T, class F>
	struct register_invoker
	{
		register_invoker()
		{
			F::template Register<T>();
		}
	};

	////////////////////////////////////////////////////////////////////////////////////////////////
	template<class T, class F>
	struct register_helper
	{
		static const register_invoker<T, F> _registrationInvoker;
	};

	template<class T, class F>
	const register_invoker<T, F> register_helper<T, F>::_registrationInvoker
		= register_invoker<T, F>();

	struct register_with_factory
	{
		template<class T>
		static void Register()
		{
			factory5::Register(T::id, [] { return std::make_unique<T>(); });
		}
	};

	template <typename T>
	struct auto_msg_factory : register_helper<T, register_with_factory>
	{
		static const key_type id;
	};

	template <typename T>
	const key_type auto_msg_factory<T>::id = reinterpret_cast<key_type>(&register_helper<T, register_with_factory>::_registrationInvoker);

	struct message3 : public message_base, auto_msg_factory<message3>
	{
		void foo() override {
			std::cout << "message3" << std::endl;
		}
	};
}
