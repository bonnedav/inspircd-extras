/*
 * InspIRCd -- Internet Relay Chat Daemon
 *
 *   Copyright (C) 2015 Antonio Costa <bigua@brasirc.com.br>
 *   Copyright (C) 2014 Joseph Newing <jnewing@gmail.com>
 *
 * This file is part of InspIRCd.  InspIRCd is free software: you can
 * redistribute it and/or modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation, version 2.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * *
 * m_ircops.cpp  - ver1.1
 *
 *  This module was been update to core 2.0 the command allow also print out
 *  a list of current aways opers.
 *
 */


/* *
 * m_ircops.cpp - ver 1.0
 *
 * Module adds the IRCOPS command that prints out a list of current online
 * opers including their class unless they have mode +H set on themselves.
 *
 * NOTES:
 * THIS COMMAND IS NOT LIMITED TO OPERS!
 *
 * I tried to keep the formatting of the output messages as close as i could
 * to kind of fit the InspIRCd look (it looks a lot like the CHECK command)
 *
 */

/* $ModDesc: /IRCOPS command that prints out a list of current online */
/* $ModAuthor: Joseph Newing (synmuffin) */
/* $ModAuthorMail: jnewing@gmail.com */
/* $ModDepends: core 1.2-1.3 */

#include "inspircd.h"

class CommandIRCOps : public Command
{

public:

    CommandIRCOps (Module* Creator) : Command(Creator,"IRCOPS",0)
	{

	}

	CmdResult Handle (const std::vector<std::string>&, User *user)
	{
		std::string ircops_str = "249 "+std::string(user->nick) + " :";

		if (user)
		{
			user->WriteServ("%s============= Online IRC Operators =============",ircops_str.c_str());

			int total = 0, total_away = 0;
			for (std::list<User*>::iterator i = ServerInstance->Users->all_opers.begin(); i != ServerInstance->Users->all_opers.end(); i++)
			{
				if (!(*i)->IsModeSet('H'))
				{
					if(!(*i)->awaymsg.empty())
					{
						total_away++;
						user->WriteServ("%s\2%s\2 - %s - (\2Currently marked away\2)", ircops_str.c_str(), std::string((*i)->nick).c_str(), std::string((*i)->oper->NameStr()).c_str());
					}

					else
					{
						user->WriteServ("%s\2%s\2 - %s - (\2Avaliable for help\2)", ircops_str.c_str(), std::string((*i)->nick).c_str(), std::string((*i)->oper->NameStr()).c_str());

					}

					total++;
				  user->WriteServ("%s- ",ircops_str.c_str());

				}
			}


			if(total >= 1)
			{
				user->WriteServ("%sTotal: \2%d\2 %s \2%d\2 %s", ircops_str.c_str() , total, "IRCop(s) - online and", total_away, "Away(s)");
				user->WriteServ("%s================================================",ircops_str.c_str());
			}

			else
			{
				user->WriteServ("%sTotal: \2%d\2 %s ",ircops_str.c_str(),  total, "IRCop unavaliable");
			}

			 user->WriteServ("%sEnd of /IRCOPS.",ircops_str.c_str());
		}

		return CMD_SUCCESS;
	}

};

class ModuleIRCOps : public Module
{

	CommandIRCOps cmd;

public:

	ModuleIRCOps() : cmd(this){}
	void init ()
	{
		ServerInstance->Modules->AddService(cmd);
	}


	virtual Version GetVersion()
	{
		return Version("m_ircops ver 1.1", VF_VENDOR);
	}

};

MODULE_INIT(ModuleIRCOps)
