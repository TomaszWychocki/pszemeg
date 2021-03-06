#include "Antenna.h"
#include "Log.h"

#include <iostream>
#include <ctime>
#include <queue>

clock_t Antenna::transmission_start_time;
std::queue<Colors> Antenna::transmission_queue;

Antenna::Antenna()
= default;

void Antenna::render_antenna(int clients_count)
{
    clock_t now = clock();
    double elapsed_secs = double(now - transmission_start_time) / CLOCKS_PER_SEC;
    double duration = map(transmission_queue.size(), 0, 3, 0.2, 0.05);

    if(elapsed_secs > duration)
    {
        antenna_color = Log::colors[Colors::Default];

        if(!transmission_queue.empty())
        {
            antenna_color = Log::colors[transmission_queue.front()];
            transmission_queue.pop();
        }

        transmission_start_time = clock();
        show_antenna(clients_count);
    }

}

void Antenna::set_transmitting(Colors color)
{
    transmission_queue.push(color);
}

double Antenna::map(double x, double in_min, double in_max, double out_min, double out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void Antenna::show_antenna(int clients_count)
{
    std::string antenna =
            antenna_color + "                   ,                          .                   \n" +
            antenna_color + "                 :;                            ;;                 \n" +
            antenna_color + "                ;,  ;:                      :;  .;                \n" +
            antenna_color + "               ,;  ;,  ;                   ;  .;  :,               \n" +
            antenna_color + "               '  .;  ;.  .     " + Log::colors[Colors::Red] + ",,." + Log::colors[Colors::Default] + "" + antenna_color + "     .  .;  ;.  '               \n" +
            antenna_color + "               '  ;.  '  '    " + Log::colors[Colors::Red] + "'''''''" + Log::colors[Colors::Default] + "" + antenna_color + "    '  '  `;  '`              \n" +
            antenna_color + "               '  ;.  '  '`  " + Log::colors[Colors::Red] + "'''''''''" + Log::colors[Colors::Default] + "" + antenna_color + "  `'  '  `;  '               \n" +
            antenna_color + "               '  `'  :,     " + Log::colors[Colors::Red] + "'''''''''" + Log::colors[Colors::Default] + "" + antenna_color + "    ,   ,;  '`                 \n" +
            antenna_color + "               .;  ::  ::     " + Log::colors[Colors::Default] + "," + Log::colors[Colors::Red] + "'''''" + Log::colors[Colors::Default] + "," + antenna_color + "    ::  :;  ;.               \n" +
            antenna_color + "                ::  ,;        " + Log::colors[Colors::Default] + "''' '''" + antenna_color + "       ;,  ::                " + Log::colors[Colors::Default] + "\n" +
            antenna_color + "                 ,;          " + Log::colors[Colors::Default] + ":'': ,'';" + antenna_color + "         ;,                 " + Log::colors[Colors::Default] + "\n" +
            "                             ''' `''''                            \n" +
            "                            ;'':'''''''                           \n" +
            "                           `''''''  ;''`                          \n" +
            "                           '''';     '''                          \n" +
            "                          ,'''';     ;'':                         \n" +
            "                          '''.'''',   '''                         \n" +
            "                         :'':   ;'''; ,'';                        \n" +
            "                         '''      :'''''''                        \n" +
            "                        ;''.         ''''''                       \n" +
            "                       `'''            ;'''.                      \n" +
            "                       '''           ,''''''                      \n" +
            "                      ,'';         :'''' ;'':                     \n" +
            "                      '''        ;''':    '''                     \n" +
            "                     ;'',     `'''':      ,'';                    \n" +
            "                     '''    `''''.         '''                    \n" +
            "                    '''.  ,''''            `'''                   \n" +
            "                   `''' ;''''               ;''.                  \n" +
            "                   ''';''':                  '''                  \n" +
            "                  ,''''',                    :'':                 \n" +
            "                  ''''.                       '''                 \n" +
            "                 ;'',                         .'';                \n" +
            "                 '''                           '''`               \n" +
            "                 `:`                           `:`                \n";

    antenna += "Clients count: " + std::to_string(clients_count) + "\n";

    std::cout << "\033[35A\r" << antenna << std::flush;
}


