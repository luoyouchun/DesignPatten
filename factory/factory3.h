#pragma once
#include <map>
#include <string>
#include <functional>
#include <memory>

template <typename P, typename... U>
struct factory3
{
	template<typename T>
	struct register_t
	{
		register_t(const std::string& key)
		{
			factory3<P, U...>::get().map_.emplace(key, [](U&&... u) { return new T(std::forward<U>(u)...); });
		}

		register_t(const std::string& key, std::function<P*(U&&...)>&& f)
		{
			factory3<P, U...>::get().map_.emplace(key, std::move(f));
		}
	};

	inline P* produce(const std::string& key, U&&... u)
	{
		if (map_.find(key) == map_.end())
			throw std::invalid_argument("the message key is not exist!");

		return map_[key](std::forward<U>(u)...);
	}

	inline static factory3<P, U...>& get()
	{
		static factory3<P, U...> instance;
		return instance;
	}

private:
	factory3<P, U...>() {};
	factory3<P, U...>(const factory3<P, U...>&) = delete;
	factory3<P, U...>(factory3<P, U...>&&) = delete;

	std::map<std::string, std::function<P*(U&&...)>> map_;
};

#define REGISTER_MESSAGE1(T, key, ...) static factory3<T, __VA_ARGS__>::register_t<T> REGISTER_MESSAGE_VNAME(T)(key);
#define REGISTER_MESSAGE2(T, key, f, ...) static factory3<T, __VA_ARGS__>::register_t<T> REGISTER_MESSAGE_VNAME(T)(key, f);

#define REGISTER_MESSAGE_VNAME(T) reg_msg_##T##_