#ifndef SCHED_SCHEDULED_H
#define SCHED_SCHEDULED_H

#include <string>
#include <map>

#define SPACE ' '
#define COMMA ','
#define COLON ':'

namespace sched
{
    enum AspectType : char
    {
        HandEyeCoordination = 'H',
        Endurance = 'E',
        Pizzazz = 'P',
    };

    AspectType to_aspect_type(char ch);

    typedef std::map<AspectType, long> AspectMap;

    struct data_context;

    struct scheduled {

        long id;

        std::string name;

        AspectMap aspects;

    protected:

        data_context* pdc;

        scheduled(data_context* pdc, const char* n = "");

    public:

        void set_aspect(AspectType key, long value);

        virtual bool is_game() const;

        virtual bool is_player() const;

    private:

        void init();
    };
}

#endif //SCHED_SCHEDULED_H
