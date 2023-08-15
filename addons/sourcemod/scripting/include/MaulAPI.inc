#if defined _maulapi_included
    #endinput
#endif
#define _maulapi_included

#define MAUL_INFO_NAME        "MAUL"
#define MAUL_INFO_AUTHOR      "EdgeGamers Organization"
#define MAUL_INFO_DESCRIPTION ""
#define MAUL_INFO_VERSION     "3.2.2"
#define MAUL_INFO_WEBSITE     "https://www.edge-gamers.com"

#define MAUL_CLIENT_ID32_LENGTH 32 // STEAM_1:0:00000 SteamID.
#define MAUL_CLIENT_ID64_LENGTH 32 // 64-bit SteamID.

#define MAUL_BANS_UNBANNED 0
#define MAUL_BANS_PERMABAN -1

#define MAUL_USER_NOT_REAL 0

#define MAUL_USER_DONATOR_TIER1 ADMFLAG_CUSTOM6
#define MAUL_USER_DONATOR_TIER2 ADMFLAG_CUSTOM5
#define MAUL_USER_DEBUG_ROOT    ADMFLAG_ROOT

#define COMMRANK_EG  ADMFLAG_KICK
#define COMMRANK_EGO ADMFLAG_BAN

// Required API Includes
#include <base64>
#include <ripext>

/*
 * Utility Methods
 */
stock void EncodeBase64Url(char[] output, char[] input, int len)
{
    EncodeBase64(output, len, input);

    ReplaceString(output, len, "/", "_", false);
    ReplaceString(output, len, "+", "-", false);
}

stock bool UserHasFlag(int client, int flags)
{
    return CheckCommandAccess(client, "MaulAPI_UserHasFlag_CheckCommandAccess", flags, true);
}

/*
 * API Data Objects
 */
methodmap MaulAdmin < StringMap
{
    public MaulAdmin()
    {
        return view_as<MaulAdmin>(CreateTrie());
    }

    property StringMap parent
    {
        public get()
        {
            return view_as<StringMap>(this);
        }
    }

    property int Identity
    {
        public get()
        {
            int identity;
            this.GetValue("maip_identity", identity);

            return identity;
        }
        public set(int noset) {}
    }

    property int Rank
    {
        public get()
        {
            int rank;
            this.GetValue("maip_rank", rank);

            return rank;
        }
        public set(int noset) {}
    }

    property bool Verification
    {
        public get()
        {
            bool verified;
            this.GetValue("maip_verified", verified);

            return verified;
        }
        public set(bool noset) {}
    }

    public void Name(char[] out, int len)
    {
        this.GetString("maip_name", out, len);
    }

    public void Division(char[] out, int len)
    {
        this.GetString("maip_division", out, len);
    }

    public JSONArray Groups()
    {
        JSONArray groups = null;
        return this.GetValue("maip_groups", groups) ? groups : null;
    }
}

methodmap MaulDonator < StringMap
{
    public MaulDonator()
    {
        return view_as<MaulDonator>(CreateTrie());
    }

    property StringMap parent
    {
        public get()
        {
            return view_as<StringMap>(this);
        }
    }

    property bool IsDonator
    {
        public get()
        {
            bool donator;
            this.GetValue("mdip_donator", donator);

            return donator;
        }
        public set(bool noset) {}
    }

    property int Level
    {
        public get()
        {
            int level;
            this.GetValue("mdip_level", level);

            return level;
        }
        public set(int noset) {}
    }

    public void JoinMessage(char[] out, int len)
    {
        this.GetString("mdip_join_message", out, len);
    }
}

methodmap MaulBan < StringMap
{
    public MaulBan()
    {
        return view_as<MaulBan>(CreateTrie());
    }

    property StringMap parent
    {
        public get()
        {
            return view_as<StringMap>(this);
        }
    }

    property int Duration
    {
        public get()
        {
            int duration;
            this.GetValue("mbip_duration", duration);

            return duration;
        }
        public set(int noset) {}
    }

    public void Reason(char[] out, int len)
    {
        this.GetString("mbip_reason", out, len);
    }
}

/*
 * Cross-Plugin Events
 */
forward void OnMaulApiReady();
forward void OnMaulInfoReady(int client);

/*
 * Function Callback Templates
 */
typedef ApiRequestResponse = function void (JSONObject response, int source);

/*
 * API Access Object
 */
methodmap Maul
{
    public native Maul();

    public native void LogDebug(const char[] message, any...);
    public native void LogError(const char[] message, any...);
    public native void LogMessage(const char[] message, any...);

    public native bool GetAdminInfo(const int client, MaulAdmin &hndl);
    public native bool GetDonatorInfo(const int client, MaulDonator &hndl);
    public native bool GetBanInfo(const int client, MaulBan &hndl);

    public native void CreateBan(int victim, int admin, int time, const char[] reason, const char[] notes = "");
    public native void CreateRequest(const char[] request, ApiRequestResponse callback, int source, const char[] ...);
}

public SharedPlugin __pl_maulapi =
{
    name = "Maul",
    file = "maul_api.smx",
#if defined REQUIRE_PLUGIN
    required = 1,
#else
    required = 0,
#endif
};

#if !defined REQUIRE_PLUGIN
public void __pl_maulapi_SetNTVOptional()
{
    MarkNativeAsOptional("Maul.LogDebug");
    MarkNativeAsOptional("Maul.GetDonatorInfo");
    MarkNativeAsOptional("Maul.LogMessage");

    MarkNativeAsOptional("Maul.GetAdminInfo");
    MarkNativeAsOptional("Maul.GetDonatorInfo");
    MarkNativeAsOptional("Maul.GetBanInfo");

    MarkNativeAsOptional("Maul.CreateBan");
    MarkNativeAsOptional("Maul.CreateRequest");
}
