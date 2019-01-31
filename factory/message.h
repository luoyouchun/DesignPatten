#pragma once
struct message_base {
	virtual void foo() = 0;
	virtual ~message_base() {}
};
