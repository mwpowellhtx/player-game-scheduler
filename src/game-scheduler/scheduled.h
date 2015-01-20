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

    struct scheduled {

        long id;

        std::string name;

        AspectMap aspects;

    protected:

        scheduled();

        scheduled(const char* n);

    public:

        void set_aspect(AspectType key, long value);

        virtual bool is_game() const;

        virtual bool is_player() const;
    };
}

#endif //SCHED_SCHEDULED_H
