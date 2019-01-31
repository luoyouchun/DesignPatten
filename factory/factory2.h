#pragma once
#include <map>
#include <string>
#include <functional>
#include <memory>

template<typename P>
struct factory2
{
	template<typename T>
	struct register_t
	{
		register_t(const std::string& key)
		{
			factory2::get().map_.emplace(key, &register_t<T>::create);
		}

		inline static P* create() { return new T; }
	};

	inline P* produce(const std::string& key)
	{
		if (map_.find(key) == map_.end())
			throw std::invalid_argument("the product key is not exist!");

		return map_[key]();
	}

	std::unique_ptr<P> produce_unique(const std::string& key)
	{
		return std::unique_ptr<P>(produce(key));
	}

	std::shared_ptr<P> produce_shared(const std::string& key)
	{
		return std::shared_ptr<P>(produce(key));
	}
	typedef P*(*FunPtr)();

	inline static factory2& get()
	{
		static factory2 instance;
		return instance;
	}

private:
	factory2() {};
	factory2(const factory2&) = delete;
	factory2(factory2&&) = delete;

	std::map<std::string, FunPtr> map_;
};

#define REGISTER_PRODUCT_VNAME(T) reg_product_##T##_
#define REGISTER_PRODUCT(T, key) static factory2<T>::register_t<T> REGISTER_PRODUCT_VNAME(T)(key);