#include <a_samp>
#include <Pawn.CMD>
#include <sscanf>
#include <system_jail>

public OnPlayerCommandReceived(playerid, cmd[], params[], flags)
{
	if(IsPlayerInJail(playerid))
	{
		new
		    minutes,
		    seconds,
			string[68];
		GetPlayerTimeInJail(playerid, minutes, seconds);
		if(minutes != 0)
		{
			format(string, sizeof(string), "[Cárcel]: Te falta %d minutos con %d para poder salir de la cárcel.", minutes, seconds);
			SendClientMessage(playerid, -1, string);
		}
		else
		{
			format(string, sizeof(string), "[Cárcel]: Te falta %d segundos para poder salir de la cárcel.", seconds);
			SendClientMessage(playerid, -1, string);
		}
	    return 0;
	}
	return 1;
}

public OnPlayerComplyOneMinute(playerid, const minutes)
{
	new string[46];
	format(string, sizeof(string), "[Cárcel]: En %d minutos saldrás de la cárcel.", minutes);
	SendClientMessage(playerid, -1, string);
	return 1;
}

public OnPlayerEnterJail(playerid, const minutes)
{
	new string[45];
	format(string, sizeof(string), "[Cárcel]: En %d minutos saldrás de la cárcel.", minutes);
	SendClientMessage(playerid, -1, string);
	return 1;
}

public OnPlayerLeaveJail(playerid)
{
	SendClientMessage(playerid, -1, "[Cárcel]: Fuiste libre!");
	SpawnPlayer(playerid);
	return 1;
}

main()
{

}

cmd:jail(playerid, params[])
{
	new
	    userid,
	    minutes;
	if(sscanf(params, "dd", userid, minutes))
	    return SendClientMessage(playerid, -1, "USO: /jail [playerid] [minutes]");
	if(IsPlayerInJail(userid))
	    return SendClientMessage(playerid, -1, "ERROR: Ese jugador ya está en la cárcel.");
	PutPlayerInJail(userid, minutes);
	return 1;
}

cmd:unjail(playerid, params[])
{
	new userid;
    if(sscanf(params, "d", userid))
	    return SendClientMessage(playerid, -1, "USO: /unjail [playerid]");
   	if(!IsPlayerInJail(userid))
	    return SendClientMessage(playerid, -1, "ERROR: Ese jugador no está en la cárcel.");
	RemovePlayerJail(userid);
	return 1;
}

cmd:jailed(playerid)
{
	if(GetUsersInJail() != 0)
		ShowPlayersInJail(playerid);
	else
	    SendClientMessage(playerid, -1, "ERROR: No hay jugadores en la cárcel.");
	return 1;
}
