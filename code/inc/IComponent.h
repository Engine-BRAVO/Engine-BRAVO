#pragma once

class IComponent {
public:
    virtual ~IComponent() = default;
    virtual const char* GetType() const = 0;
};
