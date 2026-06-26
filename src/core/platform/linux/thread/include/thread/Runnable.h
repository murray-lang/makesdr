#pragma once

/**
 * @brief Interface for objects that can be run by a Thread.
 * Similar to Java's Runnable interface.
 */
class Runnable
{
public:
    virtual ~Runnable() = default;

    /**
     * @brief The entry point for the thread execution.
     * This method will be called when the thread starts.
     */
    virtual void run() = 0;
};
