/*
 * Steam ID handling stocks
 * by: rtldg
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License, version 3.0, as published by the
 * Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#if defined _steamid_stocks_included
	#endinput
#endif
#define _steamid_stocks_included

static KeyValues kv = null;

// Retrieves accountid from STEAM_X:Y:Z, [U:1:123], and 765xxxxxxxxxxxxxx
stock int SteamIDToAccountID(const char[] sInput)
{
	char sSteamID[32];
	strcopy(sSteamID, sizeof(sSteamID), sInput);
	ReplaceString(sSteamID, 32, "\"", "");
	TrimString(sSteamID);

	if (StrContains(sSteamID, "STEAM_") != -1)
	{
		ReplaceString(sSteamID, 32, "STEAM_", "");

		char parts[3][11];
		ExplodeString(sSteamID, ":", parts, 3, 11);

		// Let X, Y and Z constants be defined by the SteamID: STEAM_X:Y:Z.
		// Using the formula W=Z*2+Y, a SteamID can be converted:
		return StringToInt(parts[2]) * 2 + StringToInt(parts[1]);
	}
	else if (StrContains(sSteamID, "U:1:") != -1)
	{
		ReplaceString(sSteamID, 32, "[", "");
		ReplaceString(sSteamID, 32, "U:1:", "");
		ReplaceString(sSteamID, 32, "]", "");

		return StringToInt(sSteamID);
	}
	else if (StrContains(sSteamID, "765") == 0)
	{
		return SteamID64ToAccountID(sSteamID);
	}

	return 0;
}

stock void AccountIDToSteamID64Num(int accountid, int num[2])
{
	num[0] = accountid;
	//       universe  | type      | instance
	num[1] = (1 << 24) | (1 << 20) | 1; // 0x01100001
}

stock void AccountIDToSteamID64(int accountid, char[] buf, int buflen)
{
	int num[2];
	AccountIDToSteamID64Num(accountid, num);
	SteamID64ToString(num, buf, buflen);
}

stock void AccountIDToSteamID2(int accountid, char[] buf, int buflen)
{
	FormatEx(buf, buflen, "STEAM_0:%d:%d", accountid&1, (accountid>>1) & 0x7FFFFFFF);
}

stock void AccountIDToSteamID3(int accountid, char[] buf, int buflen)
{
	FormatEx(buf, buflen, "[U:1:%d]", accountid);
}

stock void SteamID64ToString(int num[2], char[] buf, int buflen)
{
	if (kv == null)
		kv = new KeyValues("fuck sourcemod");

	kv.SetUInt64(NULL_STRING, num);
	kv.GetString(NULL_STRING, buf, buflen);
}

stock int SteamID64ToAccountID(const char[] steamid64)
{
	if (kv == null)
		kv = new KeyValues("fuck sourcemod");

	int num[2];
	kv.SetString(NULL_STRING, steamid64);
	kv.GetUInt64(NULL_STRING, num);
	return num[0]; // & 0x7FFFFFFF;
}
