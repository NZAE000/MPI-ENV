#pragma once

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>


struct TimeLogger {

    // Useful type aliases for the entire class.
    using Clk = std::chrono::system_clock;
    using ms  = std::chrono::microseconds;
	//using Duration  = std::chrono::duration<uint64_t, std::ratio<1, 1000000000>>; // std::ratio<1, 1000000000> = std::nano
	//using TimePoint = std::chrono::time_point<Clk, Duration>;
    
    explicit TimeLogger() = default;

    // Method to get current timestamp as string.
    std::string get_timestamp() const 
    {
        //using namespace std::chrono;
        auto now        = Clk::now();
        auto now_time_t = Clk::to_time_t(now);

        // Tiempo desde el epoch en microsegundos
        auto micros_total = duration_cast<ms>(now.time_since_epoch()) % std::chrono::seconds(1);

        // Separar milisegundos y microsegundos
        auto millis = micros_total.count() / 1000;
        auto micros = micros_total.count() % 1000;

        // Convertir a estructura de tiempo legible
        std::tm tm_now = *std::localtime(&now_time_t);

        // Formatear con el formato HH:MM:SS:mmm:uuu
        std::ostringstream oss;
        oss << std::put_time(&tm_now, "%H:%M:%S")
            << ':' << std::setfill('0') << std::setw(3) << millis
            << ':' << std::setfill('0') << std::setw(3) << micros;

        return oss.str();
    }
};

