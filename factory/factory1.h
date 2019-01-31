#pragma once
#include <map>
#include <string>
#include <functional>
#include <memory>

#include "message.h"

struct factory1
{
	template<typename T>
	struct register_t
	{
		register_t(const std::string& key)
		{
			factory1::get().map_.emplace(key, &register_t<T>::create);
		}

		inline static message_base* create() { return new T; }
	};

	inline message_base* produce(const std::string& key)
	{
		if (map_.find(key) == map_.end())
			throw std::invalid_argument("the product key is not exist!");

		return map_[key]();
	}

	std::unique_ptr<message_base> produce_unique(const std::string& key)
	{
		return std::unique_ptr<message_base>(produce(key));
	}

	std::shared_ptr<message_base> produce_shared(const std::string& key)
	{
		return std::shared_ptr<message_base>(produce(key));
	}
	typedef message_base*(*FunPtr)();

	inline static factory1& get()
	{
		static factory1 instance;
		return instance;
	}

private:
	factory1() {};
	factory1(const factory1&) = delete;
	factory1(factory1&&) = delete;

	std::map<std::string, FunPtr> map_;
};

#define REGISTER_MESSAGE_VNAME(T) reg_msg_##T##_
#define REGISTER_MESSAGE(T, key) static factory1::register_t<T> REGISTER_MESSAGE_VNAME(T)(key);