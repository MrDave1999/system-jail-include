# system-jail-include

Este include te permitirá hacer un sistema de cárcel de una forma sencilla y eficaz. 
Además de eso, el include le da al jugador una posición (x, y, z) + un interior de manera pre-determinada, adicional a eso, también le resetea las armas que tenga puesto el usuario.
Así que no necesitas preocuparte sí los jugadores de la cárcel están juntos, porqué el include mismo le asigna un mundo virtual diferente a cada uno de ellos.
Sí no te gusta el lugar de la cárcel que proporciona el include, usted mismo lo puede cambiar con el callback `OnPlayerEnterJail`.

En los ejemplos se usará el procesador de comandos [Pawn.CMD](https://github.com/urShadow/Pawn.CMD/releases) y para lectura de datos [sscanf](https://github.com/maddinat0r/sscanf).

## Instalación

Usted debe agregar este archivo `system_jail.inc` en el directorio /pawno/include. También deberá incluir la librería en su modo de juego.
```pawn
#include <system_jail>
```
Para que el `gamemode` compile correctamente, deberá tener las librerías [YSI](https://github.com/MrDave1999/YSI-Includes/releases/tag/v4.0.0) en la carpeta `include` (la que viene en el IDE).

El include `system_jail.inc` tiene una cantidad máxima de jugadores en la cárcel (por defecto son 10). Usted puede cambiar ese valor usando este pequeño código al principio del `gamemode`. Claro, lo hará después de la inclusión.
```pawn
#include <system_jail>
#undef MAX_JAILED
#define MAX_JAILED (5)
```

## callbacks

- `OnPlayerEnterJail` = Es invocado cuando un jugador entra a la cárcel.

**Ejemplo:**
```pawn
public OnPlayerEnterJail(playerid, const minutes)
{
	printf("El ID: %d entró a la cárcel", playerid);
	printf("En %d minutos saldrá de la cárcel", minutes);
	return 1;
}
```
También puedes detectar sí el jugador tiene un tiempo específico o no. Con el siguiente código de ejemplo:
```pawn
public OnPlayerEnterJail(playerid, const minutes)
{
	if(minutes == NO_TIME)
	{
	    printf("Entraste a la cárcel de manera definitiva");
		return 1;
	}
	printf("El ID: %d entró a la cárcel", playerid);
	printf("En %d minutos saldrá de la cárcel", minutes);
	return 1;
}
```

Usted también puede agregar una nueva posición de cárcel para ese jugador, no necesariamente debes usar la que viene por defecto en el include.
El mundo virtual del jugador no se necesita asignarlo porqué el include lo hace por ti.

**Ejemplo:**
```pawn
public OnPlayerEnterJail(playerid, const minutes)
{
	SetPlayerPos(playerid, 2125.4805,1584.6116,20.3906);
	SetPlayerInterior(playerid, 0);
	if(minutes == NO_TIME)
	{
	    printf("Entraste a la cárcel de manera definitiva");
		return 1;
	}
	printf("El ID: %d entró a la cárcel", playerid);
	printf("En %d minutos saldrá de la cárcel", minutes);
	return 1;
}
```

- `OnPlayerLeaveJail` = Es invocado cuando un jugador sale de la cárcel.

**Ejemplo:**
```pawn
public OnPlayerLeaveJail(playerid)
{
	printf("El ID: %d salió de la cárcel", playerid);
	return 1;
}
```

- `OnPlayerComplyOneMinute` = Es invocado cada vez que el jugador cumple un minuto de cárcel.

**Ejemplo:**
```pawn
public OnPlayerComplyOneMinute(playerid, const minutes)
{
	printf("El ID: %d le falta %d minutos de cárcel", playerid, minutes);
	return 1;
}
```

## Macros

- `IsPlayerInJail` = Esta macro detecta sí el jugador está en la cárcel o no.

**Ejemplo:** 
```pawn
public OnPlayerCommandReceived(playerid, cmd[], params[], flags)
{
	if(IsPlayerInJail(playerid))
	{
	    printf("El ID: %d no puede usar comandos", playerid);
	    return 0;
	}
	return 1;
}
```

- `GetUsersInJail()` = Esta te servirá para saber cuántos jugadores hay en la cárcel.

**Ejemplo:** 
```pawn
cmd:totaljail(playerid)
{
	printf("Total de jugadores en la cárcel: %d", GetUsersInJail());
	return 1;
}
```

- `GetPlayerPositionInJail` = Esta macro te permitará saber la posición en la que entró el jugador a la cárcel.

**Ejemplo:**
```pawn
cmd:position(playerid)
{
    GetPlayerPositionInJail playerid;
	printf("ID: %d se encuentra en la posición: %d", playerid, position+1);
	return 1;
}
```
La variable `position` es declarada de manera automática, así que no necesitas hacerlo tú.

## Funciones

- `GetPlayerTimeInJail` = Esta función devuelve la cantidad total de segundos que le falta al jugador para salir de la cárcel. Además de eso, también se puede obtener el tiempo de forma separada (minutos y segundos).

**Ejemplo 1:** 
```pawn
cmd:time(playerid)
{
	printf("Te falta %d segundos para salir de la cárcel", GetPlayerTimeInJail(playerid));
	return 1;
}
```

**Ejemplo 2:** 
```pawn
cmd:time(playerid)
{
	new
	    minutes,
	    seconds;
	GetPlayerTimeInJail(playerid, minutes, seconds);
	printf("Te falta 00:%02d:%02d para salir de la cárcel", minutes, seconds);
	return 1;
}
```

- `PutPlayerInJail` = Esta función pone al jugador hacia la cárcel con un tiempo (en minutos) determinado.

**Ejemplo:** 
```pawn
cmd:jail(playerid)
{
	PutPlayerInJail(playerid, 3);
	return 1;
}
```
Con la función `PutPlayerInJail` también puedes omitir el último parámetro y esto hará que el tiempo de cárcel sea ilimitado para el jugador.
```pawn
cmd:jail(playerid)
{
	PutPlayerInJail(playerid);
	return 1;
}
```

- `RemovePlayerJail` = Esta función elimina a un jugador de la cárcel.

**Ejemplo:** 
```pawn
cmd:unjail(playerid)
{
	RemovePlayerJail(playerid);
	return 1;
}
```

- `ShowPlayersInJail` = Esta función mostrará un menú de todos los jugadores que estén en la cárcel.

**Ejemplo:** 
```pawn
cmd:encarcelados(playerid)
{
	ShowPlayersInJail(playerid);
	return 1;
}
```
En el servidor debería salir así el menú:
[![system_jail](https://i.imgur.com/dS8x7QW.png)](https://github.com/MrDave1999)

O así también, sí es que hay jugadores en la cárcel sin ningún tiempo definido.
[![system_jail](https://i.imgur.com/HDmvYa3.png)](https://github.com/MrDave1999)

## Uso

Aquí te daré un ejemplo de manera general sobre como usar este `include`. 
Las librerías que usaré será:
- [sscanf](https://github.com/maddinat0r/sscanf) 
- [Pawn.CMD](https://github.com/urShadow/Pawn.CMD/releases)

```pawn
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
			string[77];
		GetPlayerTimeInJail(playerid, minutes, seconds);
		if(minutes != 0)
		{
			format(string, sizeof(string), "[Cárcel]: Te falta %d minutos con %d segundos para poder salir de la cárcel.", minutes, seconds);
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
```

## Créditos

- [MrDave](https://github.com/MrDave1999)
- [Y_Less](https://github.com/Y-Less) (por sus librerías y_timers, y_hooks)
- SA-MP Team
