#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <string>
#include <vector>
#include "driver.hpp"

class Memory {

public: 

    HANDLE driver_handle;

    Memory() {
        
    }

    template<typename T> T RPM(const UINT64 address) {
        driver::info_t io_info;
        T read_data;

        io_info.target_address = address;
        io_info.buffer_address = (UINT64)&read_data;
        io_info.size = sizeof(T);

        DeviceIoControl(driver_handle, driver::read_code, &io_info, sizeof(io_info), &io_info, sizeof(io_info), nullptr, nullptr);

        return read_data;
    }

    std::string read_game_string(uintptr_t string_ptr, size_t max_len = 128) {
        if (!string_ptr) return "";

        char* buffer = new char[max_len];
        for (int i = 0; i < max_len; i++) {
            buffer[i] = '\0';
        }

        driver::info_t io_info{};
        io_info.target_address = string_ptr;
        io_info.buffer_address = (UINT64)buffer;
        io_info.size = max_len - 1;

        if (!DeviceIoControl(driver_handle, driver::read_code, &io_info, sizeof(io_info), &io_info, sizeof(io_info), nullptr, nullptr)) {
            return "";
        }

        std::string str = std::string(buffer);
        delete[] buffer;
        return str;
    }

    template<typename T> bool WPM(const UINT64 address, const T buffer) {
        driver::info_t io_info;

        io_info.target_address = address;
        io_info.buffer_address = (UINT64)&buffer;
        io_info.size = sizeof(T);

        DeviceIoControl(driver_handle, driver::write_code, &io_info, sizeof(io_info), &io_info, sizeof(io_info), nullptr, nullptr);
        return io_info.return_size == sizeof(T);
    }

    uintptr_t get_module_base(DWORD process_id, const wchar_t* module_name)
    {
        uintptr_t module_base = 0;
        HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, process_id);

        if (snapshot == INVALID_HANDLE_VALUE) {
            return 0;
        }

        MODULEENTRY32W module_entry;
        module_entry.dwSize = sizeof(MODULEENTRY32W);

        if (Module32FirstW(snapshot, &module_entry)) {
            do {
                if (_wcsicmp(module_entry.szModule, module_name) == 0) {

                    std::wcout << L"Found module: " << module_entry.szModule << L" base: " << reinterpret_cast<uintptr_t>(module_entry.modBaseAddr) << std::endl;
                    module_base = reinterpret_cast<uintptr_t>(module_entry.modBaseAddr);
                    break;
                }
            } while (Module32NextW(snapshot, &module_entry));
        }

        CloseHandle(snapshot);
        return module_base;
    }

    DWORD get_process_id(const wchar_t* pname)
    {
        DWORD process_id = 0;
        HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (snapshot == INVALID_HANDLE_VALUE)
        {
            return 0;
        }

        PROCESSENTRY32 pe;
        pe.dwSize = sizeof(PROCESSENTRY32);
        if (Process32First(snapshot, &pe))
        {
            do
            {
                if (_wcsicmp(pe.szExeFile, pname) == 0)
                {
                    process_id = pe.th32ProcessID;
                    break;
                }
            } while (Process32Next(snapshot, &pe));
        }
        CloseHandle(snapshot);
        return process_id;
    }

};

