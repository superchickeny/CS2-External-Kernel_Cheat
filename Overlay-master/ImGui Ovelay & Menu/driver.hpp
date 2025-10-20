#pragma once
#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <string>
#include <vector>

namespace driver {

	constexpr DWORD init_code = CTL_CODE(FILE_DEVICE_UNKNOWN, 0x775, METHOD_BUFFERED, FILE_SPECIAL_ACCESS); //custom io control code for setting g_target_process by target process id
	constexpr DWORD read_code = CTL_CODE(FILE_DEVICE_UNKNOWN, 0x776, METHOD_BUFFERED, FILE_SPECIAL_ACCESS); //custom io control code for reading memory
    constexpr DWORD write_code = CTL_CODE(FILE_DEVICE_UNKNOWN, 0x777, METHOD_BUFFERED, FILE_SPECIAL_ACCESS);

    struct info_t { //message type that will be passed between user program and driver
        UINT64 target_pid = 0; //process id of process we want to read from / write to
        UINT64 target_address = 0x0; //address in the target proces we want to read from / write to
        UINT64 buffer_address = 0x0; //address in our usermode process to copy to (read mode) / read from (write mode)
        UINT64 size = 0; //size of memory to copy between our usermode process and target process
        UINT64 return_size = 0; //number of bytes successfully read / written
        PVOID module_base = 0;
    };

    HANDLE get_driver_handle() 
    {
        return CreateFile(L"\\\\.\\erickyD", GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    }

    BOOL attach_to_process(DWORD process_id, HANDLE driver_handle) {
        info_t io_info;

        io_info.target_pid = process_id;

        return DeviceIoControl(driver_handle, init_code, &io_info, sizeof(io_info), &io_info, sizeof(io_info), nullptr, nullptr);
    }


}