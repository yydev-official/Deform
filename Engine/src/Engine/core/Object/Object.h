// Object.h

#pragma once

#include <cstdint>
#include <EngineAPI.h>

class DEFORM_API Object
{

public:

    // Constructor and overloads
    Object() = delete;
    explicit Object(uint64_t id);

    // Destructor
    virtual ~Object() = default;

    // State tracking
    void Destroy();
    bool GetPendingDestroy() const;

    // Getter method for Object ID
    uint64_t GetID() const;

private:

    uint64_t m_ID;
    bool m_PendingDestroy;

};