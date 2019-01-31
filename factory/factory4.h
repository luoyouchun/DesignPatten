#pragma once
#include <memory>
#include <map>
#include <functional>
#include <iostream>
#include <cassert>
#include "message.h"

class factory4 {
public:
	factory4() = delete;
	using CreateMethod = std::function<std::unique_ptr<message_base>()>;

	static bool Register(std::string id, CreateMethod creator)
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

	static std::unique_ptr<message_base> produce(std::string id)
	{
		auto& map = get_map();
		auto it = map.find(id);
		if (it != map.end())
			return it->second();

		return nullptr;
	}

	static std::map<std::string, CreateMethod>& get_map() {
		static std::map<std::string, CreateMethod> map;
		return map;
	}
};

template <typename T>
class auto_msg_factory
{
protected:
	static bool registered_;
};

template <typename T>
bool auto_msg_factory<T>::registered_ = factory4::Register(T::Id(), [] { return std::make_unique<T>(); });

struct message1 : public message_base, auto_msg_factory<message1>{
	static std::string Id() {
		registered_;
		//assert(!registered_);
		return "msg1";
	}

	void foo() override {
		std::cout << "message1" << std::endl;
	}
};

struct message2 : public message_base, auto_msg_factory<message2>{
	static std::string Id() {
		assert(!registered_);
		return "msg2";
	}

	void foo() override {
		std::cout << "message2" << std::endl;
	}
};