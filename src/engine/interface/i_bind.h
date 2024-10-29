#pragma once

namespace engine {

class IBind {
public:
	virtual ~IBind() = default;

	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;
};

}