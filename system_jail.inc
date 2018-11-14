#if !defined _INC_system_jail
	#define _INC_system_jail
#else
	#endinput
#endif

//Includes
#include <a_samp>
#include <YSI\y_timers>
#include <YSI\y_hooks>

#define MAX_JAILED  			(10)
#define NO_TIME                 (-1)
#define DIALOG_JAILED           (9878)

#define GetUsersInJail()        UsersInJail
#define IsPlayerInJail(%0)      (GetPlayerTeam(%0) == IN_JAIL)
#define GetPlayerPositionInJail%0; new position = 0; for(; position != UsersInJail; ++position) if((%0) == Jail[position][USER_ID_JAIL])break;

//Forward declarations
forward OnPlayerEnterJail(playerid, const minutes);
forward OnPlayerLeaveJail(playerid);

#if defined Y@@_OnPlayerEnterJail
	forward Y@@_OnPlayerEnterJail(playerid, const minutes);
#endif

#if defined Y@@_OnPlayerLeaveJail
	forward Y@@_OnPlayerLeaveJail(playerid);
#endif

#if defined OnPlayerComplyOneMinute
	forward OnPlayerComplyOneMinute(playerid, const minutes);
#endif

enum E_JAIL
{
	USER_ID_JAIL,
	MINUTES_X,
	SECONDS_X
};

new Jail[MAX_JAILED][E_JAIL];
new Timer:ID_TimeJail;
new UsersInJail;

const IN_JAIL = 206;

hook OnPlayerDisconnect(playerid, reason)
{
	if(IsPlayerInJail(playerid))
		RemovePlayerJail(playerid);
	return 1;
}

public OnPlayerEnterJail(playerid, const minutes)
{
	ResetPlayerWeapons(playerid);
    SetPlayerPos(playerid, 197.6661, 173.8179, 1003.0234);
	SetPlayerInterior(playerid, 3);
	SetPlayerVirtualWorld(playerid, playerid+1);
	SetPlayerSpecialAction(playerid, SPECIAL_ACTION_CUFFED);
	#if defined Y@@_OnPlayerEnterJail
		return Y@@_OnPlayerEnterJail(playerid, minutes);
	#else
	    return 1;
	#endif
}

public OnPlayerLeaveJail(playerid)
{
	SetPlayerVirtualWorld(playerid, 0);
	SetPlayerInterior(playerid, 0);
	#if defined Y@@_OnPlayerLeaveJail
		return Y@@_OnPlayerLeaveJail(playerid);
	#else
	    return 1;
	#endif
}

stock ShowPlayersInJail(playerid)
{
	new string[30+(45*MAX_JAILED)];
	new title[31];
	new name[MAX_PLAYER_NAME];
	strcat(string, "Nombre\tID\tTiempo Faltante\n");
	for(new i = 0; i != UsersInJail; ++i)
	{
	    GetPlayerName(Jail[i][USER_ID_JAIL], name, sizeof(name));
		if(Jail[i][MINUTES_X] == NO_TIME)
			format(string, sizeof(string), "%s%s\t%d\tInfinito\n", string, name, Jail[i][USER_ID_JAIL]);
		else
	    	format(string, sizeof(string), "%s%s\t%d\t00:%02d:%02d\n", string, name, Jail[i][USER_ID_JAIL], Jail[i][MINUTES_X]-1, Jail[i][SECONDS_X]);
	}
	format(title, sizeof(title), "Usuarios en la cárcel: >> %d <<", UsersInJail);
	ShowPlayerDialog(playerid, DIALOG_JAILED, DIALOG_STYLE_TABLIST_HEADERS, title, string, "Cerrar", "");
	PlayerPlaySound(playerid, 1139, 0, 0, 0);
}

stock GetPlayerTimeInJail(playerid, &minutes = 0, &seconds = 0)
{
    GetPlayerPositionInJail playerid; 
    minutes = Jail[position][MINUTES_X]-1;
    if(minutes == NO_TIME-1)
    {
        minutes = -1;
        return -1;
    }
	seconds = Jail[position][SECONDS_X];
	return ((minutes*60)+seconds);
}

stock PutPlayerInJail(playerid, const time = -1)
{
	SetPlayerTeam(playerid, IN_JAIL);
	++UsersInJail;
	OnPlayerEnterJail(playerid, Jail[UsersInJail-1][MINUTES_X] = time);
	Jail[UsersInJail-1][SECONDS_X] = 60;
	Jail[UsersInJail-1][USER_ID_JAIL] = playerid;
	if(UsersInJail == 1)
 		ID_TimeJail = repeat TimeJail();
}

RemovePlayerJail(playerid)
{
	SetPlayerTeam(playerid, NO_TEAM);
    OnPlayerLeaveJail(playerid);
    GetPlayerPositionInJail playerid; 
	for(new i = position+1; i != UsersInJail; ++i)
	{
	    Jail[i-1][USER_ID_JAIL] = Jail[i][USER_ID_JAIL];
	    Jail[i-1][MINUTES_X] = Jail[i][MINUTES_X];
	    Jail[i-1][SECONDS_X] = Jail[i][SECONDS_X];
	}
	--UsersInJail;
	if(UsersInJail == 0)
	    stop ID_TimeJail;
}

timer TimeJail[1000]()
{
	for(new i = 0; i != UsersInJail; ++i)
	{
	    if(Jail[i][MINUTES_X] == NO_TIME) continue;
		if(--Jail[i][SECONDS_X] == 0)
		{
		    Jail[i][SECONDS_X] = 60;
			if(--Jail[i][MINUTES_X] == 0)
			{
				RemovePlayerJail(Jail[i][USER_ID_JAIL]);
				return 1;
			}
			#if defined OnPlayerComplyOneMinute
				OnPlayerComplyOneMinute(Jail[i][USER_ID_JAIL], Jail[i][MINUTES_X]);
			#endif
		}
	}
	return 1;
}

//Hooks callbacks
//==================================================
#if !defined _ALS_OnPlayerEnterJail
	#define _ALS_OnPlayerEnterJail
#else
	#undef OnPlayerEnterJail
#endif
#define OnPlayerEnterJail Y@@_OnPlayerEnterJail
//==================================================
#if !defined _ALS_OnPlayerLeaveJail
	#define _ALS_OnPlayerLeaveJail
#else
	#undef OnPlayerLeaveJail
#endif
#define OnPlayerLeaveJail Y@@_OnPlayerLeaveJail
//==================================================
