#include <node.h>

#ifdef _WIN32
#include <Windows.h>

void getActiveWindow(const v8::FunctionCallbackInfo<v8::Value>& args){
    HWND activeWindowHandle = GetForegroundWindow();

    int titleLength = GetWindowTextLengthW(activeWindowHandle);
    auto* title = (wchar_t*)malloc(sizeof(wchar_t)*titleLength+1);
    GetWindowTextW(activeWindowHandle, title, titleLength+1);

    DWORD procId = 0;
    GetWindowThreadProcessId(activeWindowHandle, &procId);

    auto* moduleName = (wchar_t*)malloc(sizeof(wchar_t)*MAX_PATH+1);
    DWORD moduleNameSz = MAX_PATH+1;
    HANDLE procHandle = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, false, procId);
    if(procHandle){
        QueryFullProcessImageNameW(procHandle, 0, moduleName, &moduleNameSz);
    }

    v8::Isolate* isolate = args.GetIsolate();
    {
        v8::HandleScope scope(isolate);

        v8::Local<v8::Object> ret = v8::Object::New(isolate);
        ret->Set(
            args.GetIsolate()->GetCurrentContext(),
            v8::String::NewFromUtf8(isolate, "title", v8::NewStringType::kNormal).ToLocalChecked(),
            v8::String::NewFromTwoByte(isolate, (const uint16_t*) title, v8::NewStringType::kNormal).ToLocalChecked()
        );
        ret->Set(
            args.GetIsolate()->GetCurrentContext(),
            v8::String::NewFromUtf8(isolate, "process_id", v8::NewStringType::kNormal).ToLocalChecked(),
            v8::Number::New(isolate, procId)
        );
        
        if(procHandle){
            ret->Set(
                args.GetIsolate()->GetCurrentContext(),
                v8::String::NewFromUtf8(isolate, "process_path", v8::NewStringType::kNormal).ToLocalChecked(),
                v8::String::NewFromTwoByte(isolate, (const uint16_t*) moduleName, v8::NewStringType::kNormal).ToLocalChecked()
            );
        }

        args.GetReturnValue().Set(ret);
    }

    CloseHandle(procHandle);
    free(moduleName);
    free(title);
}

void module_init(v8::Local<v8::Object> exports){
    NODE_SET_METHOD(exports, "getActiveWindow", getActiveWindow);
}
#else
void module_init(v8::Local<v8::Object> exports){}
#endif

NODE_MODULE(winapi, module_init);
