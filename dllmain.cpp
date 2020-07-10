#include <cstdint>
#include <windows.h>

namespace nfs {
    constexpr uintptr_t write = 0x821E30;
    constexpr uintptr_t read = 0x820C70;

    using write_t = void(*)(const char* udata, const char* ukey, const char* uvalue, uint32_t id, uint32_t data);
    using read_t = uint32_t(*)(const char* udata, const char* ukey, const char* uvalue, uint32_t id);

    enum class Identifier : uint32_t
    {
        CASH = 0x402FD74F
    };

    void add_user_data(Identifier id, uint32_t data) noexcept
    {
        const auto write = reinterpret_cast<nfs::write_t>(nfs::write);
        write("UserData", "UDKey", "UDValue", static_cast<uint32_t>(id), data);
    }

    [[nodiscard]] uint32_t read_user_data(Identifier id) noexcept
    {
        const auto read = reinterpret_cast<nfs::read_t>(nfs::read);
        return read("UserData", "UDKey", "UDValue", static_cast<uint32_t>(id));
    }

    void edit_user_data(Identifier id, uint32_t data) noexcept
    {
        const int32_t store = read_user_data(id);
        add_user_data(id, -store + data);
    }
}

bool APIENTRY DllMain([[maybe_unused]] HMODULE module, DWORD reason, [[maybe_unused]] void* reserved)
{
    if (reason == DLL_PROCESS_ATTACH)
        nfs::edit_user_data(nfs::Identifier::CASH, 1'000'000'000);

    return true;
}