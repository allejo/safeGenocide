/*
 * Copyright (C) 2019 Vladimir "allejo" Jimenez
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "bzfsAPI.h"

// Define plugin name
const std::string PLUGIN_NAME = "Safe Genocide";

// Define plugin version numbering
const int MAJOR = 1;
const int MINOR = 0;
const int REV = 0;
const int BUILD = 2;

class SafeGenocide : public bz_Plugin
{
public:
    const char* Name();
    void Init(const char* config);
    void Cleanup();
    void Event(bz_EventData* eventData);

private:
    int calcGenoPoints(bz_eTeamType victimTeam);
};

BZ_PLUGIN(SafeGenocide)

const char* SafeGenocide::Name()
{
    static std::string pluginName;

    if (pluginName.empty())
    {
        pluginName = bz_format("%s %d.%d.%d (%d)", PLUGIN_NAME.c_str(), MAJOR, MINOR, REV, BUILD);
    }

    return pluginName.c_str();
}

void SafeGenocide::Init(const char* config)
{
    Register(bz_ePlayerDieEvent);

    bz_RegisterCustomFlag("SG", "Safe Genocide", "It's like Genocide but without the mass killing", 0, eGoodFlag);
}

void SafeGenocide::Cleanup()
{
    Flush();
}

void SafeGenocide::Event(bz_EventData* eventData)
{
    switch (eventData->eventType)
    {
        case bz_ePlayerDieEvent:
        {
            bz_PlayerDieEventData_V2 *data = (bz_PlayerDieEventData_V2*)eventData;

            bz_eTeamType victimTeam = data->team;

            int killerID = data->killerID;
            bz_eTeamType killerTeam = data->killerTeam;

            if (data->flagKilledWith == "SG")
            {
                int genoPoints = calcGenoPoints(victimTeam);

                if (killerTeam == victimTeam && killerTeam != eRogueTeam)
                {
                    bz_incrementPlayerLosses(killerID, genoPoints);
                }
                else
                {
                    bz_incrementPlayerWins(killerID, genoPoints);
                }
            }
        }
        break;

        default:
            break;
    }
}

int SafeGenocide::calcGenoPoints(bz_eTeamType victimTeam)
{
    int points = 0;
    bz_APIIntList* playerList = bz_getPlayerIndexList();

    for (unsigned int i = 0; i < playerList->size(); i++)
    {
        int playerID = playerList->get(i);
        bz_BasePlayerRecord *pr = bz_getPlayerByIndex(playerID);

        if (pr->team == victimTeam && pr->spawned)
        {
            points++;
        }

        bz_freePlayerRecord(pr);
    }

    bz_deleteIntList(playerList);

    return points;
}
