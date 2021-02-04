/*
* Tencent is pleased to support the open source community by making Puerts available.
* Copyright (C) 2020 THL A29 Limited, a Tencent company.  All rights reserved.
* Puerts is licensed under the BSD 3-Clause License, except for the third-party components listed in the file 'LICENSE' which may be subject to their corresponding license terms.
* This file is subject to the terms and conditions defined in file 'LICENSE', which is part of this source code package.
*/

#include "JSFunction.h"
#include "V8Utils.h"
#include "JSEngine.h"

namespace puerts
{
    JSFunction::JSFunction(v8::Isolate* InIsolate, v8::Local<v8::Context> InContext, v8::Local<v8::Function> InFunction, int32_t InIndex)
    {
        ResultInfo.Isolate = InIsolate;
        ResultInfo.Context.Reset(InIsolate, InContext);
        GFunction.Reset(InIsolate, InFunction);
        Index = InIndex;
    }

    JSFunction::~JSFunction()
    {
        v8::Isolate* Isolate = ResultInfo.Isolate;
        v8::Isolate::Scope IsolateScope(Isolate);
        v8::HandleScope HandleScope(Isolate);
        v8::Local<v8::Context> Context = ResultInfo.Context.Get(Isolate);
        v8::Context::Scope ContextScope(Context);

        auto Function = GFunction.Get(Isolate);

        Function->Set(Context, FV8Utils::V8String(Isolate, FUNCTION_INDEX_KEY), v8::Undefined(Isolate));

        GFunction.Reset();
        ResultInfo.Result.Reset();
        ResultInfo.Context.Reset();
    }

    /*void JSFunction::SetResult(v8::MaybeLocal<v8::Value> maybeValue)
    {

    }*/

    bool JSFunction::Invoke(bool HasResult)
    {
        v8::Isolate* Isolate = ResultInfo.Isolate;
        v8::Isolate::Scope IsolateScope(Isolate);
        v8::HandleScope HandleScope(Isolate);
        v8::Local<v8::Context> Context = ResultInfo.Context.Get(Isolate);
        v8::Context::Scope ContextScope(Context);

        std::vector< v8::Local<v8::Value>> V8Args;
        for (int i = 0; i < Arguments.size(); ++i)
        {
            V8Args.push_back(Arguments[i].Get(Isolate));
        }
        v8::TryCatch TryCatch(Isolate);
        auto maybeValue = GFunction.Get(Isolate)->Call(Context, Context->Global(), static_cast<int>(V8Args.size()), V8Args.data());
        Arguments.clear();
        if (TryCatch.HasCaught())
        {
            LastExceptionInfo = FV8Utils::ExceptionToString(Isolate, TryCatch);
            return false;
        }
        else
        {
            if (HasResult && !maybeValue.IsEmpty())
            {
                ResultInfo.Result.Reset(Isolate, maybeValue.ToLocalChecked());
            }
            return true;
        }
    }
}