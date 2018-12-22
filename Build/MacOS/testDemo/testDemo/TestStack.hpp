//
//  TestStack.hpp
//  TestDemo
//
//  Created by Zhou,Xuguang on 17/7/8.
//  Copyright © 2017年 Zhou,Xuguang. All rights reserved.
//

#ifndef TestStack_hpp
#define TestStack_hpp

#include <stdio.h>
#include <exception>
#include <stack>
#include <mutex>
#include <memory>

struct empty_stack: std::exception
{
    const char* what() const throw();
};

template <typename T>
class thrwadsafe_stack
{
    std::stack<T> data;
    mutable std::mutex m;
    
public:
    thrwadsafe_stack() {}
    
    thrwadsafe_stack(const thrwadsafe_stack& other)
    {
        std::lock_guard<std::mutex> lock(other.m);
        data = other.data;
    }
    
    thrwadsafe_stack& operator = (const thrwadsafe_stack&) = delete;
    
    void push(T newValue)
    {
        std::lock_guard<std::mutex> lock(m);
        data.push(std::move(newValue));
    }
    
    std::shared_ptr<T> pop()
    {
        std::lock_guard<std::mutex> lock(m);
        if (data.empty())
        {
            throw empty_stack();
        }
        
        std::shared_ptr<T> const res(std::make_shared<T>(std::move(data.top())));
        data.pop();
        return res;
    }
    
    void pop(T& value)
    {
        std::lock_guard<std::mutex> lock(m);
        
        if (data.empty())
        {
            throw empty_stack();
        }
        
        value = std::move(data.top());
        data.pop();
    }
    
    bool empty() const
    {
        std::lock_guard<std::mutex> lock(m);
        return data.empty();
    }
};

#endif /* TestStack_hpp */
