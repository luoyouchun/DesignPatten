#include <iostream>
#include "message.h"
#include "factory1.h"
#include "factory2.h"
#include "AbstractFactory.h"
#include "factory4.h"
#include "factory5.h"
#include "factory6.h"

struct messageA : public message_base {
	void foo() override {
		std::cout << "messageA" << std::endl;
	}
};

struct messageB : public message_base {
	void foo() override {
		std::cout << "messageB" << std::endl;
	}
};

REGISTER_MESSAGE(messageA, "msgA");
REGISTER_MESSAGE(messageB, "msgB");

void test_msg_factory() {
	auto msgA = factory1::get().produce("msgA");
	auto msgB = factory1::get().produce("msgB");
	msgA->foo();
	msgB->foo();
}


struct animal {
	virtual ~animal() {}
	virtual void foo() = 0;
};
struct dog : public animal {
	void foo() override {
		std::cout << "dog" << std::endl;
	}
};

struct cat : public animal {
	void foo() override {
		std::cout << "cat" << std::endl;
	}
};

REGISTER_PRODUCT(messageA, "msga");
REGISTER_PRODUCT(messageB, "msgb");
REGISTER_PRODUCT(dog, "dog");
REGISTER_PRODUCT(cat, "cat");

void test_abstract_factory() {
	auto msga = factory2<messageA>::get().produce("msga");
	auto msgb = factory2<messageB>::get().produce("msgb");
	auto c = factory2<cat>::get().produce("cat");
	auto d = factory2<dog>::get().produce("dog");

	msga->foo();
	msgb->foo();
	c->foo();
	d->foo();
}

struct messageC : public message_base {
	messageC(int a):a_(a){}

	void foo() override {
		std::cout << "messageC" << std::endl;
	}

	int a_;
};

REGISTER_FACTORY(messageC, "msgC", int);

void test_factory_with_args() {
	auto msgc = AbstractFactory<messageC, int>::get().produce("msgC", 2);
	msgc->foo();
}

void test_auto_factory() {
	auto msg1 = factory4::produce("msg1");
	if (msg1) {
		msg1->foo();
	}

	auto msg2 = factory4::produce("msg2");
	if (msg2) {
		msg2->foo();
	}
}

void test_auto_factory1() {
	auto msg5 = factory::factory5::produce(factory::message3::id);
	if (msg5) {
		msg5->foo();
	}
}

void test_auto_factory2() {
	auto msg4 = factory6<int>::produce(message4::id, 2);
	if (msg4) {
		msg4->foo();
	}
}

int main() {
	test_msg_factory();
	test_abstract_factory();
	test_factory_with_args();
	test_auto_factory();
	test_auto_factory1();
	test_auto_factory2();
}