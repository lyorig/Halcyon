#include <halcyon/halcyon.hpp>
#include <thread>

class app
{
public:
    bool update()
    {
        using clock = std::chrono::steady_clock;
        using namespace std::chrono_literals;

        const auto start = clock::now();

        while (m_evt.poll())
        {
            switch (m_evt.data.type)
            {
                using enum hal::event::type;

            case quit_request:
                return false;

            case key_press:
                if (m_evt.data.key.keysym.scancode == SDL_SCANCODE_Q)
                {
                    m_evt.data.type = quit_request;
                    m_evt.push();
                }
                break;
            }
        }

        std::this_thread::sleep_until(start + 16ms);

        return true;
    }

private:
    LYO_NOSIZE hal::cleanup m_cleanup;

    hal::event m_evt;

    hal::window   m_window { "HalTest", { 640, 480 }, {} };
    hal::renderer m_renderer { m_window, { hal::renderer::accelerated } };
};

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

    app a;

    while (a.update())
        ;

    return EXIT_SUCCESS;
}