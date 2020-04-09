/*	SCCS Id: @(#)allmain.c	3.4	2003/04/02	*/
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/* NetHack may be freely redistributed.  See license for details. */

/* various code that was replicated in *main.c */

#include "hack.h"
#include "edog.h"
#include "artifact.h"
#ifdef OVLB
#include "artilist.h"
#else
STATIC_DCL struct artifact artilist[];
#endif


#ifndef NO_SIGNAL
#include <signal.h>
#endif

#ifdef POSITIONBAR
STATIC_DCL void NDECL(do_positionbar);
#endif

STATIC_DCL void NDECL(androidUpkeep);
STATIC_DCL int NDECL(do_inheritor_menu);
STATIC_DCL void NDECL(see_nearby_monsters);
STATIC_DCL void NDECL(cthulhu_mind_blast);
STATIC_DCL void FDECL(unseen_actions, (struct monst *));
STATIC_DCL void FDECL(blessed_spawn, (struct monst *));
STATIC_DCL void FDECL(good_neighbor, (struct monst *));
STATIC_DCL void FDECL(dark_pharaoh, (struct monst *));
STATIC_DCL void FDECL(polyp_pickup, (struct monst *));
STATIC_DCL void FDECL(goat_sacrifice, (struct monst *));

#ifdef OVL0

extern const int monstr[];

STATIC_OVL void
digcrater(mtmp)
	struct monst *mtmp;
{
	int x,y,i,j;
	struct trap *ttmp;
	for(i=-5;i<=5;i++){
		x = mtmp->mx+i;
		for(j=-5;j<=5;j++){
			y = mtmp->my+j;
			if(isok(x,y)&& !(x == u.ux && y == u.uy)){
				ttmp = t_at(x, y);
				if(levl[x][y].typ <= SCORR || levl[x][y].typ == CORR || levl[x][y].typ == ROOM){
					if(dist2(x,y,mtmp->mx,mtmp->my) <= 26){
						levl[x][y].typ = CORR;
						if(!does_block(x,y,&levl[x][y])) unblock_point(x,y);	/* vision:  can see through */
						if(ttmp) delfloortrap(ttmp);
						if(dist2(x,y,mtmp->mx,mtmp->my) <= 9) levl[x][y].typ = LAVAPOOL;
						else if(dist2(x,y,mtmp->mx,mtmp->my) <= 26) digactualhole(x, y, mtmp, PIT, TRUE, FALSE);
					}
				}
			}
		}
	}
}

STATIC_OVL void
digcloudcrater(mtmp)
	struct monst *mtmp;
{
	int x,y,i,j;
	struct trap *ttmp;
	for(i=-2;i<=2;i++){
		x = mtmp->mx+i;
		for(j=-2;j<=2;j++){
			y = mtmp->my+j;
			if(isok(x,y) && dist2(x,y,mtmp->mx,mtmp->my) < 8){
				levl[x][y].typ = CLOUD;
			}
		}
	}
}

STATIC_OVL void
digXchasm(mtmp)
	struct monst *mtmp;
{
	int x,y;
	struct trap *ttmp;
	int inty = rn2(4)+2;
	int dy = rn2(2) ? 1 : -1;
	y = mtmp->my;
	x = mtmp->mx;
	if(isok(x,y)&& !(x == u.ux && y == u.uy)){
		ttmp = t_at(x, y);
		if(levl[x][y].typ <= SCORR || levl[x][y].typ == CORR || levl[x][y].typ == ROOM){
			levl[x][y].typ = CORR;
			if(!does_block(x,y,&levl[x][y])) unblock_point(x,y);	/* vision:  can see through */
			if(ttmp) delfloortrap(ttmp);
			digactualhole(x, y, mtmp, HOLE, TRUE, FALSE);
		}
	}
	y=y+1;
	if(isok(x,y)){
		ttmp = t_at(x, y);
		if(levl[x][y].typ <= SCORR || levl[x][y].typ == CORR || levl[x][y].typ == ROOM){
			levl[x][y].typ = CORR;
			if(!does_block(x,y,&levl[x][y])) unblock_point(x,y);	/* vision:  can see through */
			if(ttmp) delfloortrap(ttmp);
			digactualhole(x, y, mtmp, PIT, TRUE, FALSE);
		}
	}
	y=y-1;
	y=y-1;
	if(isok(x,y)){
		ttmp = t_at(x, y);
		if(levl[x][y].typ <= SCORR || levl[x][y].typ == CORR || levl[x][y].typ == ROOM){
			levl[x][y].typ = CORR;
			if(!does_block(x,y,&levl[x][y])) unblock_point(x,y);	/* vision:  can see through */
			if(ttmp) delfloortrap(ttmp);
			digactualhole(x, y, mtmp, PIT, TRUE, FALSE);
		}
	}
	y=y+1;
	for(x = mtmp->mx + 1; x < COLNO; x++){
		if(!(x%inty)) y += dy;
		if(isok(x,y) && !(x == u.ux && y == u.uy)){
			ttmp = t_at(x, y);
			if(levl[x][y].typ <= SCORR || levl[x][y].typ == CORR || levl[x][y].typ == ROOM){
				levl[x][y].typ = CORR;
				if(!does_block(x,y,&levl[x][y])) unblock_point(x,y);	/* vision:  can see through */
				if(ttmp) delfloortrap(ttmp);
				digactualhole(x, y, mtmp, HOLE, TRUE, FALSE);
			}
		}
		y=y+1;
		if(isok(x,y)){
			ttmp = t_at(x, y);
			if(levl[x][y].typ <= SCORR || levl[x][y].typ == CORR || levl[x][y].typ == ROOM){
				levl[x][y].typ = CORR;
				if(!does_block(x,y,&levl[x][y])) unblock_point(x,y);	/* vision:  can see through */
				if(ttmp) delfloortrap(ttmp);
				digactualhole(x, y, mtmp, PIT, TRUE, FALSE);
			}
		}
		y=y-1;
		y=y-1;
		if(isok(x,y)){
			ttmp = t_at(x, y);
			if(levl[x][y].typ <= SCORR || levl[x][y].typ == CORR || levl[x][y].typ == ROOM){
				levl[x][y].typ = CORR;
			if(!does_block(x,y,&levl[x][y])) unblock_point(x,y);	/* vision:  can see through */
				if(ttmp) delfloortrap(ttmp);
				digactualhole(x, y, mtmp, PIT, TRUE, FALSE);
			}
		}
		y=y+1;
	}
	y = mtmp->my;
	for(x = mtmp->mx - 1; x >= 0; x--){
		if(!(x%inty)) y -= dy;
		if(isok(x,y)&& !(x == u.ux && y == u.uy)){
			ttmp = t_at(x, y);
			if(levl[x][y].typ <= SCORR || levl[x][y].typ == CORR || levl[x][y].typ == ROOM){
				levl[x][y].typ = CORR;
			if(!does_block(x,y,&levl[x][y])) unblock_point(x,y);	/* vision:  can see through */
				if(ttmp) delfloortrap(ttmp);
				digactualhole(x, y, mtmp, HOLE, TRUE, FALSE);
			}
		}
		y=y+1;
		if(isok(x,y)){
			ttmp = t_at(x, y);
			if(levl[x][y].typ <= SCORR || levl[x][y].typ == CORR || levl[x][y].typ == ROOM){
				levl[x][y].typ = CORR;
			if(!does_block(x,y,&levl[x][y])) unblock_point(x,y);	/* vision:  can see through */
				if(ttmp) delfloortrap(ttmp);
				digactualhole(x, y, mtmp, PIT, TRUE, FALSE);
			}
		}
		y=y-1;
		y=y-1;
		if(isok(x,y)){
			ttmp = t_at(x, y);
			if(levl[x][y].typ <= SCORR || levl[x][y].typ == CORR || levl[x][y].typ == ROOM){
				levl[x][y].typ = CORR;
			if(!does_block(x,y,&levl[x][y])) unblock_point(x,y);	/* vision:  can see through */
				if(ttmp) delfloortrap(ttmp);
				digactualhole(x, y, mtmp, PIT, TRUE, FALSE);
			}
		}
		y=y+1;
	}
}

STATIC_OVL void
digYchasm(mtmp)
	struct monst *mtmp;
{
	int x,y;
	struct trap *ttmp;
	int intx = rn2(4)+2;
	int dx = rn2(2) ? 1 : -1;
	x = mtmp->mx;
	y = mtmp->my;
	if(isok(x,y)&& !(x == u.ux && y == u.uy)){
		ttmp = t_at(x, y);
		if(levl[x][y].typ <= SCORR || levl[x][y].typ == CORR || levl[x][y].typ == ROOM){
			levl[x][y].typ = CORR;
			if(!does_block(x,y,&levl[x][y])) unblock_point(x,y);	/* vision:  can see through */
			if(ttmp) delfloortrap(ttmp);
			digactualhole(x, y, mtmp, HOLE, TRUE, FALSE);
		}
	}
	x=x+1;
	if(isok(x,y)){
		ttmp = t_at(x, y);
		if(levl[x][y].typ <= SCORR || levl[x][y].typ == CORR || levl[x][y].typ == ROOM){
			levl[x][y].typ = CORR;
			if(!does_block(x,y,&levl[x][y])) unblock_point(x,y);	/* vision:  can see through */
			if(ttmp) delfloortrap(ttmp);
			digactualhole(x, y, mtmp, PIT, TRUE, FALSE);
		}
	}
	x=x-1;
	x=x-1;
	if(isok(x,y)){
		ttmp = t_at(x, y);
		if(levl[x][y].typ <= SCORR || levl[x][y].typ == CORR || levl[x][y].typ == ROOM){
			levl[x][y].typ = CORR;
			if(!does_block(x,y,&levl[x][y])) unblock_point(x,y);	/* vision:  can see through */
			if(ttmp) delfloortrap(ttmp);
			digactualhole(x, y, mtmp, PIT, TRUE, FALSE);
		}
	}
	x=x+1;
	for(y = mtmp->my + 1; y < COLNO; y++){
		if(!(y%intx)) x += dx;
		if(isok(x,y)&& !(x == u.ux && y == u.uy)){
			ttmp = t_at(x, y);
			if(levl[x][y].typ <= SCORR || levl[x][y].typ == CORR || levl[x][y].typ == ROOM){
				levl[x][y].typ = CORR;
			if(!does_block(x,y,&levl[x][y])) unblock_point(x,y);	/* vision:  can see through */
				if(ttmp) delfloortrap(ttmp);
				digactualhole(x, y, mtmp, HOLE, TRUE, FALSE);
			}
		}
		x=x+1;
		if(isok(x,y)){
			ttmp = t_at(x, y);
			if(levl[x][y].typ <= SCORR || levl[x][y].typ == CORR || levl[x][y].typ == ROOM){
				levl[x][y].typ = CORR;
			if(!does_block(x,y,&levl[x][y])) unblock_point(x,y);	/* vision:  can see through */
				if(ttmp) delfloortrap(ttmp);
				digactualhole(x, y, mtmp, PIT, TRUE, FALSE);
			}
		}
		x=x-1;
		x=x-1;
		if(isok(x,y)){
			ttmp = t_at(x, y);
			if(levl[x][y].typ <= SCORR || levl[x][y].typ == CORR || levl[x][y].typ == ROOM){
				levl[x][y].typ = CORR;
			if(!does_block(x,y,&levl[x][y])) unblock_point(x,y);	/* vision:  can see through */
				if(ttmp) delfloortrap(ttmp);
				digactualhole(x, y, mtmp, PIT, TRUE, FALSE);
			}
		}
		x=x+1;
	}
	x = mtmp->mx;
	for(y = mtmp->my - 1; y >= 0; y--){
		if(!(y%intx)) x -= dx;
		if(isok(x,y)&& !(x == u.ux && y == u.uy)){
			ttmp = t_at(x, y);
			if(levl[x][y].typ <= SCORR || levl[x][y].typ == CORR || levl[x][y].typ == ROOM){
				levl[x][y].typ = CORR;
			if(!does_block(x,y,&levl[x][y])) unblock_point(x,y);	/* vision:  can see through */
				if(ttmp) delfloortrap(ttmp);
				digactualhole(x, y, mtmp, HOLE, TRUE, FALSE);
			}
		}
		x=x+1;
		if(isok(x,y)){
			ttmp = t_at(x, y);
			if(levl[x][y].typ <= SCORR || levl[x][y].typ == CORR || levl[x][y].typ == ROOM){
				levl[x][y].typ = CORR;
			if(!does_block(x,y,&levl[x][y])) unblock_point(x,y);	/* vision:  can see through */
				if(ttmp) delfloortrap(ttmp);
				digactualhole(x, y, mtmp, PIT, TRUE, FALSE);
			}
		}
		x=x-1;
		x=x-1;
		if(isok(x,y)){
			ttmp = t_at(x, y);
			if(levl[x][y].typ <= SCORR || levl[x][y].typ == CORR || levl[x][y].typ == ROOM){
				levl[x][y].typ = CORR;
			if(!does_block(x,y,&levl[x][y])) unblock_point(x,y);	/* vision:  can see through */
				if(ttmp) delfloortrap(ttmp);
				digactualhole(x, y, mtmp, PIT, TRUE, FALSE);
			}
		}
		x=x+1;
	}
}
static int prev_hp_notify;

char *
hpnotify_format_str(char *str)
{
    static char buf[128];
    char *f, *p, *end;
    int ispercent = 0;

    buf[0] = '\0';

    if (!str) return NULL;

    f = str;
    p = buf;
    end = buf + sizeof(buf) - 10;

    while (*f) {
      if (ispercent) {
	switch (*f) {
	case 'a':
	    snprintf (p, end + 1 - p, "%d", abs(uhp()-prev_hp_notify));
	  while (*p != '\0')
	    p++;
	  break;
        case 'c':
	    snprintf (p, end + 1 - p, "%c", (prev_hp_notify > uhp() ? '-' : '+'));
	  p++;
	  break;
	case 'm':
	    snprintf (p, end + 1 - p, "%d", uhpmax());
	  while (*p != '\0')
	    p++;
	  break;
	case 'H':
	    if (uhp() == uhpmax()) {
	    snprintf (p, end + 1 - p, "%s", "max");
	  } else {
		snprintf (p, end + 1 - p, "%d", uhp());
	  }
	  while (*p != '\0')
	    p++;
	  break;
	case 'h':
	    snprintf (p, end + 1 - p, "%d", uhp());
	  while (*p != '\0')
	    p++;
	  break;
	default:
	  *p = *f;
	  if (p < end)
	    p++;
	}
	ispercent = 0;
      } else {
	if (*f == '%')
	  ispercent = 1;
	else {
	  *p = *f;
	  if (p < end)
	    p++;
	}
      }
      f++;
    }
    *p = '\0';

    return buf;
}

STATIC_OVL
void
androidUpkeep()
{
	//Pay unusual upkeep here, possibly pass out
	if(uandroid && !u.usleep){
		int mult = HEALCYCLE/u.ulevel;
		//Possibly pass out if you begin this step with 0 energy.
		if(u.uen == 0 && !rn2(10+u.ulevel) && moves >= u.nextsleep){
			int t = rn1(u.uenmax*mult+40, u.uenmax*mult+40);
			You("pass out from exhaustion!");
			u.nextsleep = moves+rnz(350)+t;
			u.lastslept = moves;
			fall_asleep(-t, TRUE);
			nomul(-1*u.uenmax, "passed out from exhaustion");
		}
		if(u.phasengn){
			losepw(10);
			if(u.uen <= 0){
				You("can no longer maintain phase!");
				u.phasengn = 0;
			}
		}
		if(u.ucspeed == HIGH_CLOCKSPEED){
			losepw(1);
			if(u.uen <= 0){
				You("can no longer maintain emergency speed!");
				u.ucspeed = NORM_CLOCKSPEED;
			}
		}
		//Exceeding operational time
		static char fatigue_warning = 0;
		if(moves >= u.nextsleep+1400 && u.uen > 10 && !fatigue_warning){
			fatigue_warning = 1;
			You_feel("fatigued.");
		}
		static char e_fatigued = 0;
		if(moves >= u.nextsleep+1400 && u.uen > 1 && u.uen <= 10 && !e_fatigued){
			e_fatigued = 1;
			You_feel("extremely fatigued.");
		}
		static char pass_warning = 0;
		if(moves >= u.nextsleep && u.uen <= 1 && !pass_warning){
			pass_warning = 1;
			You_feel("like you're about to pass out.");
		}
		if(moves > u.nextsleep+1400 && u.uen > 0){
			if(!(moves%20)) losepw(1);
		}
	}
}

static long prev_dgl_extrainfo = 0;

/* perform 1 turn's worth of time-dependent hp modification, mostly silently */
/* NOTES: can rehumanize(), can print You("pass out from exertion!") if moving when overloaded at 1 hp */
void
you_regen_hp()
{
	int wtcap = near_capacity();
	int perX = 0;
	int * hpmax;
	int * hp;

	// set hp, maxhp pointers
	hp    = (Upolyd) ? (&u.mh)    : (&u.uhp);
	hpmax = (Upolyd) ? (&u.mhmax) : (&u.uhpmax);

	if (u.uinvulnerable || u.spiritPColdowns[PWR_PHASE_STEP] >= moves + 20) {
		/* for the moment at least, you're in tiptop shape */
		/* do not regenerate any health */
		/* you also do not lose health (e.g. from being a fish out of water) */
		return;
	}

	// Sanity check
	if (Upolyd && (*hp < 1))
		rehumanize();

	//Androids regenerate from active Hoon and healing doll, 
	////but not from other sources unless dormant
	if(u.uhoon_duration && (*hp) < (*hpmax)){
		flags.botl = 1;
		
		(*hp) += 10;

		if ((*hp) > (*hpmax))
			(*hp) = (*hpmax);
	}
	if(RapidHealing && (*hp) < (*hpmax)){
		//1/5th max hp
		(*hp) += (*hpmax)/5+1;
		
		if ((*hp) > (*hpmax))
			(*hp) = (*hpmax);
	}
	
	perX += u.uhoon;
	
	//Androids regenerate from active Hoon, but not from other sources unless dormant
	// Notably, no bonus from passive Hoon
	if(uandroid && !u.usleep)
		return;
	
	// fish out of water
	if (youracedata->mlet == S_EEL && !is_pool(u.ux, u.uy, youracedata->msize == MZ_TINY) && !Is_waterlevel(&u.uz)) {
		if (is_pool(u.ux, u.uy, TRUE))
			perX -= HEALCYCLE * (youracedata->msize - 1) / (youracedata->msize);
		else
			perX -= HEALCYCLE;
		u.regen_blocked++;
	}

	// invidiaks out of dark
	if (youracedata->mtyp == PM_INVIDIAK && !isdark(u.ux, u.uy)) {
		perX -= HEALCYCLE;
		u.regen_blocked++;
	}
	
	// regeneration 'trinsic
	if (Regeneration){
		perX += HEALCYCLE;
	}
	// Corruption thought
	if(active_glyph(CORRUPTION) && (*hp < (*hpmax)*.3)){
		perX += HEALCYCLE;
	}
	
	// "Natural" regeneration has stricter limitations
	if (u.regen_blocked > 0) u.regen_blocked--;		// not regen_blocked (NOTE: decremented here)
	else if (!(nonliving(youracedata) && !uandroid) &&	// not nonliving, however, androids auto-repair while asleep
		!Race_if(PM_INCANTIFIER) &&					// not incantifier (including while polymorphed)
		(wtcap < MOD_ENCUMBER || !u.umoved) &&		// not overloaded
		!(uwep && uwep->oartifact == ART_ATMA_WEAPON && uwep->lamplit && !Drain_resistance && rn2(4)) // 3 in 4 chance of being prevented by Atma Weapon
			// Question for Chris: what if instead, do "reglevel /= 4;" when atma weapon is active, placed either before or after the minimum reg 1 check?
		)
	{
		int reglevel = u.ulevel;

		// CON bonus (while in natural form)
		if (!Upolyd)
			reglevel += ((int)ACURR(A_CON) - 10) / 2;
		// minimum 1
		if (reglevel < 1)
			reglevel = 1;

		// Healer role bonus
		if (Role_if(PM_HEALER) && !Upolyd)
			reglevel += 10;
		// Barbarian role bonus
		else if (Role_if(PM_BARBARIAN) && !Upolyd)
			reglevel *= 3;
		// Melee roles can be slightly less concerned about taking hits
		else if(!Upolyd && (
			Role_if(PM_KNIGHT)
			|| Role_if(PM_PIRATE)
			|| Role_if(PM_SAMURAI)
			|| Role_if(PM_VALKYRIE)
			|| Role_if(PM_MONK)
			|| (u.sealsActive&SEAL_BERITH)
		))
			reglevel *= 2;

		// penalty for being itchy
		reglevel -= u_healing_penalty();

		// minimum 1
		if (reglevel < 1)
			reglevel = 1;

		perX += reglevel;
	}

	// The Ring of Hygiene's Disciple
	if (!Upolyd &&	// Question for Chris: should this be enabled to also work while polymorphed?
		((uleft  && uleft->oartifact == ART_RING_OF_HYGIENE_S_DISCIPLE) ||
		(uright && uright->oartifact == ART_RING_OF_HYGIENE_S_DISCIPLE))
		){
		perX += HEALCYCLE * min(4, (*hpmax) / max((*hp), 1));
	}

	// Buer
	if (u.sealsActive&SEAL_BUER){
		int dsize = spiritDsize();

		if (uwep && uwep->oartifact == ART_ATMA_WEAPON && uwep->lamplit && !Drain_resistance)
			perX += dsize * 6 / 4;
		else
			perX += dsize * 6;
	}
	
	/* moving around while encumbered is hard work */
	if (wtcap > MOD_ENCUMBER && u.umoved) {
		if (*hp > 1)
		{
			if (wtcap < EXT_ENCUMBER)
				perX -= 1;
			else
				perX -= 3;
		}
		else
		{
			You("pass out from exertion!");
			exercise(A_CON, FALSE);
			fall_asleep(-10, FALSE);
		}
	}

	if (((perX > 0) && ((*hp) < (*hpmax))) ||			// if regenerating
		((perX < 0))									// or dying
		)
	{
		// update bottom line
		flags.botl = 1;

		// modify by 1/HEALCYCLEth of perX per turn:
		*hp += perX / HEALCYCLE;
		// Now deal with any remainder
		if (((moves)*(abs(perX) % HEALCYCLE)) / HEALCYCLE >((moves - 1)*(abs(perX) % HEALCYCLE)) / HEALCYCLE)
			*hp += 1 * sgn(perX);
		// cap at maxhp
		if ((*hp) > (*hpmax))
			(*hp) = (*hpmax);

		// check for rehumanization
		if (Upolyd && (*hp < 1))
			rehumanize();
	}
}

/* perform 1 turn's worth of time-dependent power modification, silently */
void
you_regen_pw()
{
	int wtcap = near_capacity();
	int perX = 0;

	// natural power regeneration
	if (wtcap < MOD_ENCUMBER &&		// not overly encumbered
		!Race_if(PM_INCANTIFIER)	// not an incantifier
		) {
		int reglevel = u.ulevel + (((int)ACURR(A_WIS)) - 10) / 2;
		// level + WISmod minimum 1
		if (reglevel < 1)
			reglevel = 1;

		// role bonuses
		if (Role_if(PM_WIZARD))   reglevel += 10;
		if (Role_if(PM_HEALER))   reglevel += 6;
		if (Role_if(PM_PRIEST))   reglevel += 6;
		if (Role_if(PM_VALKYRIE)) reglevel += 3;
		if (Role_if(PM_MONK))     reglevel += 3;

		// cornuthaum bonus for wizards (but not incantifiers, since they don't naturally regenerate power at all)
		if (u.uen < u.uenmax && (Role_if(PM_WIZARD)) && uarmh && uarmh->otyp == CORNUTHAUM){
			reglevel += uarmh->spe;
		}
		// penalty for being itchy
		reglevel -= u_healing_penalty();
		// penalty from spell protection interfering with natural pw regeneration
		if (u.uspellprot > 0)
			reglevel -= 10 + 2 * u.uspellprot;

		// minimum 1
		if (reglevel < 1)
			reglevel = 1;

		perX += reglevel;
	}

	// external power regeneration
	if (Energy_regeneration ||										// energy regeneration 'trinsic
		(u.umartial && !uarmf && IS_GRASS(levl[u.ux][u.uy].typ))	// or being a bare-foot martial-artist standing on grass
		)
	{
		perX += HEALCYCLE;
	}

	// Unknown God
	if (u.specialSealsActive&SEAL_UNKNOWN_GOD){
		perX += spiritDsize() * 6;
	}

	// power drain from maintained spells
	if (u.maintained_en_debt > 0)
	{
		int reglevel = u.maintained_en_debt / 3;
		int debtpaid = 0;

		if (perX > reglevel)
		{// can just subtract drain from pw regeneration and still have net positive
			perX -= reglevel;
			debtpaid += reglevel;
		}
		else
		{// either 0 pw regen or net drain
			// put the entirety of perX against the debt owed
			reglevel -= perX;
			debtpaid += perX;
			perX = 0;
			// drain further if the player has reserves to burn
			if (u.uen > 0 || Race_if(PM_INCANTIFIER))
			{
				debtpaid += reglevel;
				perX = -reglevel;
			}
		}
		// settle the payment
		u.maintained_en_debt -= debtpaid / HEALCYCLE;
		//Now deal with any remainder
		if (((moves)*(debtpaid % HEALCYCLE)) / HEALCYCLE >((moves - 1)*(debtpaid % HEALCYCLE)) / HEALCYCLE)
			u.maintained_en_debt -= 1;
		// minimum zero
		if (u.maintained_en_debt < 0)
			u.maintained_en_debt = 0;
	}

	//Naen fast energy regen applies to androids, too.
	if(u.unaen_duration && (u.uen < u.uenmax)){
		flags.botl = 1;
		u.uen += 10;
		if (u.uen > u.uenmax)
			u.uen = u.uenmax;
	}
	
	//Naen passive bonus only partially applies to androids and incantifiers
	if(Race_if(PM_INCANTIFIER)){
		if(perX < 0)
			perX += u.unaen;
		if(perX > 0)
			perX = 0;
	}
	else perX += u.unaen;
	
	//Androids only regen power while asleep, but allow their rate to offset spell maintenance
	if(uandroid && !u.usleep && perX > 0)
		return;

	if (((perX > 0) && (u.uen < u.uenmax)) ||							// if regenerating power
		((perX < 0) && ((u.uen > 0) || Race_if(PM_INCANTIFIER)))		// or draining power
		)
	{
		//update bottom line
		flags.botl = 1;

		// modify by 1/HEALCYCLEth of perX per turn:
		u.uen += perX / HEALCYCLE;
		// Now deal with any remainder
		if (((moves)*(abs(perX) % HEALCYCLE)) / HEALCYCLE >((moves - 1)*(abs(perX) % HEALCYCLE)) / HEALCYCLE)
			u.uen += 1 * sgn(perX);
		// cap at maxpw
		if (u.uen > u.uenmax)
			u.uen = u.uenmax;
		// and at 0 (for non-incantifiers)
		if (u.uen < 0 && !(Race_if(PM_INCANTIFIER)))
			u.uen = 0;
	}
}

void
moveloop()
{
#if defined(MICRO) || defined(WIN32)
	char ch;
#endif
	int abort_lev, i, j;
    struct monst *mtmp, *nxtmon;
	struct obj *pobj;
    int moveamt = 0, wtcap = 0, change = 0;
    boolean didmove = FALSE, monscanmove = FALSE;
	int oldspiritAC=0;
	int tx,ty;
	int nmonsclose,nmonsnear,enkispeedlim;
	static boolean oldBlind = 0, oldLightBlind = 0;
	int healing_penalty = 0;
    int hpDiff;
	static int oldsanity = 100;

    flags.moonphase = phase_of_the_moon();
    if(flags.moonphase == FULL_MOON) {
	You("are lucky!  Full moon tonight.");
	change_luck(1);
    } else if(flags.moonphase == NEW_MOON) {
	pline("Be careful!  New moon tonight.");
    }
    flags.friday13 = friday_13th();
    if (flags.friday13) {
	pline("Watch out!  Bad things can happen on Friday the 13th.");
	change_luck(-1);
    }

    initrack();


    /* Note:  these initializers don't do anything except guarantee that
	    we're linked properly.
    */
    decl_init();
    monst_init();
    monstr_init();	/* monster strengths */
    objects_init();

    commands_init();
#ifdef WIZARD
	// Called twice in dNAO?
    //if (wizard) add_debug_extended_commands();
#endif

    (void) encumber_msg(); /* in case they auto-picked up something */

    u.uz0.dlevel = u.uz.dlevel;
    youmonst.movement = NORMAL_SPEED;	/* give the hero some movement points */
	flags.move = FALSE; /* From nethack 3.6.2 */
    prev_hp_notify = uhp();

	if(u.ualignbase[A_ORIGINAL] == A_LAWFUL && flags.initalign != 0){
		flags.initalign = 0;
		impossible("Bad alignment initializer detected and fixed. Save and reload.");
	}
	if(u.ualignbase[A_ORIGINAL] == A_NEUTRAL && flags.initalign != 1){
		flags.initalign = 1;
		impossible("Bad alignment initializer detected and fixed. Save and reload.");
	}
	if(u.ualignbase[A_ORIGINAL] == A_CHAOTIC && flags.initalign != 2){
		flags.initalign = 2;
		impossible("Bad alignment initializer detected and fixed. Save and reload.");
	}
    for(;;) {/////////////////////////MAIN LOOP/////////////////////////////////
    hpDiff = u.uhp;
	get_nh_event();
#ifdef POSITIONBAR
	do_positionbar();
#endif

	didmove = flags.move;
	if(didmove) {
	    /* actual time passed */
		if(flags.run != 0){
			if(uwep && uwep->oartifact == ART_TENSA_ZANGETSU){
				youmonst.movement -= 1;
			} else if(uwep && uwep->oartifact == ART_SODE_NO_SHIRAYUKI){
				youmonst.movement -= 3;
			} else if(uandroid && u.ucspeed == HIGH_CLOCKSPEED){
				youmonst.movement -= 3;
			} else if(uwep && uwep->oartifact == ART_TOBIUME){
				youmonst.movement -= 4;
			} else {
				youmonst.movement -= NORMAL_SPEED;
			}
		} else if(uandroid && u.ucspeed == HIGH_CLOCKSPEED){
			if(u.umoved){
				youmonst.movement -= 3;
			} else {
				u.ucspeed = NORMAL_SPEED;
				youmonst.movement -= NORMAL_SPEED;
			}
		} else youmonst.movement -= NORMAL_SPEED;
		
		  /**************************************************/
		 /*monsters that respond to the player turn go here*/
		/**************************************************/
		for (mtmp = fmon; mtmp; mtmp = nxtmon){
			nxtmon = mtmp->nmon;
			if(mtmp->m_insight_level > u.uinsight){
				insight_vanish(mtmp);
				continue;
			}
			if(mtmp->mfaction == DELOUSED){
				delouse_tame(mtmp);
				continue;
			}
			if (!DEADMONSTER(mtmp)
				&& attacktype(mtmp->data, AT_WDGZ)
				&& !(controlledwidegaze(mtmp->data) && (mtmp->mpeaceful || mtmp->mtame))
				&& !(hideablewidegaze(mtmp->data) && (rn2(3) < magic_negation(mtmp)))
				&& couldsee(mtmp->mx, mtmp->my)
			) m_respond(mtmp);
		}
		if((is_ice(u.ux, u.uy) || mad_turn(MAD_COLD_NIGHT)) && roll_madness(MAD_FRIGOPHOBIA)){
			if(!Flying && !Levitation){
				You("panic from setting foot on ice!");
				nomul(-1*rnd(6), "panicking");
			}
			else if(roll_madness(MAD_FRIGOPHOBIA)){//Very scared of ice
				You("panic from traveling over ice!");
				nomul(-1*rnd(3), "panicking");
			}
		}
		
	    do { /* hero can't move this turn loop */
		wtcap = encumber_msg();

		flags.mon_moving = TRUE;
		do { /* Monster turn loop */
		    monscanmove = movemon();
			  /****************************************/
			 /*once-per-monster-moving things go here*/
			/****************************************/
////////////////////////////////////////////////////////////////////////////////////////////////
			if(u.specialSealsActive&SEAL_LIVING_CRYSTAL)
				average_dogs();
			for (mtmp = fmon; mtmp; mtmp = nxtmon){
				nxtmon = mtmp->nmon;
				if(mtmp->mtyp == PM_HELLCAT){
					if(!isdark(mtmp->mx,mtmp->my) && !mtmp->minvis){
						mtmp->minvis = TRUE;
						mtmp->perminvis = TRUE;
						newsym(mtmp->mx,mtmp->my);
					} else if(isdark(mtmp->mx,mtmp->my) && mtmp->minvis){
						mtmp->minvis = FALSE;
						mtmp->perminvis = FALSE;
						newsym(mtmp->mx,mtmp->my);
					}
				} else if(mtmp->mtyp == PM_HUDOR_KAMEREL){
					if(is_pool(mtmp->mx,mtmp->my, TRUE) && !mtmp->minvis){
						mtmp->minvis = TRUE;
						mtmp->perminvis = TRUE;
						newsym(mtmp->mx,mtmp->my);
					} else if(!is_pool(mtmp->mx,mtmp->my, TRUE) && mtmp->minvis){
						mtmp->minvis = FALSE;
						mtmp->perminvis = FALSE;
						newsym(mtmp->mx,mtmp->my);
					}
				} else if(mtmp->mtyp == PM_GRUE || mtmp->mtyp == PM_INVIDIAK){
					if(isdark(mtmp->mx,mtmp->my) && !mtmp->minvis){
						mtmp->minvis = TRUE;
						mtmp->perminvis = TRUE;
						newsym(mtmp->mx,mtmp->my);
					} else if(!isdark(mtmp->mx,mtmp->my) && mtmp->minvis){
						mtmp->minvis = FALSE;
						mtmp->perminvis = FALSE;
						newsym(mtmp->mx,mtmp->my);
					}
				}
				if(mtmp->m_insight_level > u.uinsight){
					insight_vanish(mtmp);
					continue;
				}
				if(mtmp->mfaction == DELOUSED){
					delouse_tame(mtmp);
					continue;
				}
				if (mtmp->minvis){
					newsym(mtmp->mx, mtmp->my);
				}
				if (Blind && Bloodsense && has_blood_mon(mtmp)){
					newsym(mtmp->mx, mtmp->my);
				}
				if(uwep && uwep->oartifact == ART_SINGING_SWORD && !is_deaf(mtmp)){
					//quite noisy
					mtmp->mux = u.ux;
					mtmp->muy = u.uy;
				}
			}
////////////////////////////////////////////////////////////////////////////////////////////////
			if(Echolocation){
				for(i=1; i<COLNO; i++)
					for(j=0; j<ROWNO; j++)
						if(viz_array[j][i]&COULD_SEE)
							echo_location(i, j);
				see_monsters();
			}
			/*If anything a monster did caused us to get moved out of water, surface*/
			if(u.usubwater && !is_pool(u.ux, u.uy, FALSE)){
				u.usubwater = 0;
				vision_full_recalc = 1;
				vision_recalc(2);	/* unsee old position */
				doredraw();
			} else if(Is_waterlevel(&u.uz) && u.usubwater && !is_3dwater(u.ux, u.uy)){
				You("pop out into an airbubble!");
				u.usubwater = 0;
			} else if(Is_waterlevel(&u.uz) && !u.usubwater && is_3dwater(u.ux, u.uy)){
				Your("%s goes under water!", body_part(HEAD));
				if(!Breathless) You("can't breath.");
				u.usubwater = 1;
			}
////////////////////////////////////////////////////////////////////////////////////////////////
			switch (((u_healing_penalty() - healing_penalty) > 0) - ((u_healing_penalty() - healing_penalty) < 0))
			{
			case 0:
					break;
			case 1:
					if (!Hallucination) {
						You_feel("%s.", (healing_penalty) ? "itchier" : "itchy");
					}
					else {
						You_feel("%s.", (healing_penalty) ? "uglier" : "ugly");
					}
					healing_penalty = u_healing_penalty();
					break;
			case -1:
					if (!Hallucination) {
						You_feel("%s.", (u_healing_penalty()) ? "some relief" : "relieved");
					}
					else {
						You_feel("%s.", (u_healing_penalty()) ? "pretty" : "beautiful");
					}
					healing_penalty = u_healing_penalty();
					break;
			}
			/*
			if (healing_penalty != u_healing_penalty()) {
				if (!Hallucination){
					You_feel("%s.", (!healing_penalty) ? "itchy" : "relief");
				} else {
					You_feel("%s.", (!healing_penalty) ? (hates_silver(youracedata) ? "tarnished" :
						hates_iron(youracedata) ? "magnetic" : "like you are failing Organic Chemistry") : "like you are no longer failing Organic Chemistry");
				}
				
				
			}
			*/
////////////////////////////////////////////////////////////////////////////////////////////////
			if (!oldBlind ^ !Blind) {  /* one or the other but not both */
				see_monsters();
				flags.botl = 1;
				vision_full_recalc = 1;	/* blindness just got toggled */
				if (Blind_telepat || Infravision) see_monsters();
				oldBlind = !!Blind;
			}
////////////////////////////////////////////////////////////////////////////////////////////////
			if (!oldLightBlind ^ !LightBlind) {  /* one or the other but not both */
				see_monsters();
				flags.botl = 1;
				vision_full_recalc = 1;	/* blindness just got toggled */
				if (Blind_telepat || Infravision) see_monsters();
				oldLightBlind = !!LightBlind;
			}
////////////////////////////////////////////////////////////////////////////////////////////////
			calc_total_maxhp();
////////////////////////////////////////////////////////////////////////////////////////////////
			calc_total_maxen();
////////////////////////////////////////////////////////////////////////////////////////////////
			oldspiritAC = u.spiritAC;
			u.spiritAC = 0; /* reset temporary spirit AC bonuses. Do this once per monster turn */
			u.spiritAttk = 0;
			if(u.sealsActive){
				if(u.sealsActive&SEAL_MALPHAS){
					for (mtmp = fmon; mtmp; mtmp = mtmp->nmon){
						if(mtmp->mtyp == PM_CROW && mtmp->mtame && !um_dist(mtmp->mx,mtmp->my,1)){
							u.spiritAC++;
							u.spiritAttk++;
						}
					}
				}
				if(u.sealsActive&SEAL_ANDREALPHUS){
					if(isok(tx=u.ux-1,ty=u.uy-1) && IS_CORNER(levl[tx][ty].typ) 
						&& IS_WALL(levl[tx+1][ty].typ) && IS_WALL(levl[tx][ty+1].typ)) u.spiritAC += u.ulevel/2+1;
					else if(isok(tx=u.ux+1,ty=u.uy-1) && IS_CORNER(levl[tx][ty].typ) 
						&& IS_WALL(levl[tx-1][ty].typ) && IS_WALL(levl[tx][ty+1].typ)) u.spiritAC += u.ulevel/2+1;
					else if(isok(tx=u.ux+1,ty=u.uy+1) && IS_CORNER(levl[tx][ty].typ) 
						&& IS_WALL(levl[tx-1][ty].typ) && IS_WALL(levl[tx][ty-1].typ)) u.spiritAC += u.ulevel/2+1;
					else if(isok(tx=u.ux-1,ty=u.uy+1) && IS_CORNER(levl[tx][ty].typ) 
						&& IS_WALL(levl[tx+1][ty].typ) && IS_WALL(levl[tx][ty-1].typ)) u.spiritAC += u.ulevel/2+1;
				}
				if(u.sealsActive&SEAL_ENKI){
					for(tx=u.ux-1; tx<=u.ux+1;tx++){
						for(ty=u.uy-1; ty<=u.uy+1;ty++){
							if(isok(tx,ty) && (tx!=u.ux || ty!=u.uy) && IS_STWALL(levl[tx][ty].typ)) u.spiritAC += 1;
						}
					}
				}
				if(u.spiritAC!=oldspiritAC) flags.botl = 1;
			}
////////////////////////////////////////////////////////////////////////////////////////////////
			if(u.uhs == WEAK && u.sealsActive&SEAL_AHAZU) unbind(SEAL_AHAZU,TRUE);
#ifndef GOLDOBJ
			// if(u.sealsActive&SEAL_FAFNIR && u.ugold < u.ulevel*100) unbind(SEAL_FAFNIR,TRUE);
			if(u.sealsActive&SEAL_FAFNIR && u.ugold == 0) unbind(SEAL_FAFNIR,TRUE);
#else
			// if(u.sealsActive&SEAL_FAFNIR && money_cnt(invent) < u.ulevel*100) unbind(SEAL_FAFNIR,TRUE);
			if(u.sealsActive&SEAL_FAFNIR && money_cnt(invent) == 0) unbind(SEAL_FAFNIR,TRUE);
#endif
			if(u.sealsActive&SEAL_JACK && (Is_astralevel(&u.uz) || Inhell)) unbind(SEAL_JACK,TRUE);
			if(u.sealsActive&SEAL_ORTHOS && !(Darksight || Catsight || Extramission)
				&&!(
					(viz_array[u.uy][u.ux]&TEMP_LIT3 && !(viz_array[u.uy][u.ux]&TEMP_DRK3)) || 
					(levl[u.ux][u.uy].lit && !(viz_array[u.uy][u.ux]&TEMP_DRK3 && !(viz_array[u.uy][u.ux]&TEMP_LIT3)))
				   )
			){
				if(Elfsight){
					if(++u.orthocounts>(5*3)) unbind(SEAL_ORTHOS,TRUE);
				} else if(Lowlightsight){
					if(++u.orthocounts>(5*2)) unbind(SEAL_ORTHOS,TRUE);
				} else {
					if(++u.orthocounts>5) unbind(SEAL_ORTHOS,TRUE);
				}
				if(u.sealsActive&SEAL_ORTHOS && Hallucination){ /*Didn't just unbind it*/
					if(u.orthocounts == 1) pline("It is now pitch black. You are likely to be eaten by a grue.");
					else pline("You are likely to be eaten by a grue.");
				} else You_feel("increasingly panicked about being in the dark!");
			    if (multi >= 0) {
					if (occupation)
						stop_occupation();
					else
						nomul(0, NULL);
				}
			}
			if(u.sealsActive&SEAL_NABERIUS && u.udrunken < u.ulevel/3) unbind(SEAL_NABERIUS,TRUE);
			if(u.specialSealsActive&SEAL_NUMINA && u.ulevel<30) unbind(SEAL_SPECIAL|SEAL_NUMINA,TRUE);
			if(u.sealsActive&SEAL_SHIRO && uarmc && uarmc->otyp == MUMMY_WRAPPING){
				struct obj *otmp = uarmc;
				pline("Hey, people might notice me with that!");
				if (donning(otmp)) cancel_don();
				(void) Cloak_off();
				useup(otmp);
				unbind(SEAL_SHIRO,TRUE);
			}
////////////////////////////////////////////////////////////////////////////////////////////////
			//These artifacts may need to respond to what monsters have done.
			///If the player no longer meets the kusanagi's requirements (ie, they lost the amulet)
			///blast 'em and drop to the ground.
			if (uwep && uwep->oartifact == ART_KUSANAGI_NO_TSURUGI){
				if(!touch_artifact(uwep,&youmonst, FALSE)){
					dropx(uwep);
				}
			}
			///If the Atma weapon becomes cursed, it drains life from the wielder to stay lit.
			if(uwep && uwep->oartifact == ART_ATMA_WEAPON &&
				uwep->cursed && 
				uwep->lamplit
			){
				if (!Drain_resistance) {
					pline("%s for a moment, then %s brightly.",
					  Tobjnam(uwep, "flicker"), otense(uwep, "burn"));
					losexp("life force drain",TRUE,TRUE,TRUE);
					uwep->cursed = FALSE;
				}
			}
////////////////////////////////////////////////////////////////////////////////////////////////
			see_nearby_monsters();
////////////////////////////////////////////////////////////////////////////////////////////////
		    if (youmonst.movement >= NORMAL_SPEED)
			break;	/* it's now your turn */
		} while (monscanmove);
		flags.mon_moving = FALSE;

		if (!monscanmove && youmonst.movement < NORMAL_SPEED) {
		    /* both you and the monsters are out of steam this round */
		    /* set up for a new turn */
		    mcalcdistress();	/* adjust monsters' trap, blind, etc */

		    /* reallocate movement rations to monsters */
			flags.goldka_level=0;
			flags.silence_level=0;
			flags.spore_level=0;
			flags.slime_level=0;
			flags.walky_level=0;
			flags.shade_level=0;
		    for (mtmp = fmon; mtmp; mtmp = nxtmon){
				nxtmon = mtmp->nmon;
				/* check for bad swap weapons */
				if (mtmp->msw) {
					struct obj *obj;

					for(obj = mtmp->minvent; obj; obj = obj->nobj)
						if (obj->owornmask & W_SWAPWEP) break;
					if (!obj || mtmp->msw != obj){
						MON_NOSWEP(mtmp);
						impossible("bad monster swap weapon detected (and fixed)");
					}
				}
				/* Some bugs cause mtame and mpeaceful to diverge, fix w/ a warning */
				if(mtmp->mtame && !mtmp->mpeaceful){
					impossible("Hostile+tame monster state detected (and fixed)");
					mtmp->mpeaceful = TRUE;
				}
				//Remove after testing (can cause "re-trapping" of untrapped monsters)
				if(!mtmp->mtrapped && t_at(mtmp->mx, mtmp->my) && t_at(mtmp->mx, mtmp->my)->ttyp == VIVI_TRAP && !DEADMONSTER(mtmp)){
					impossible("Re-trapping mon %s in vivi trap",noit_mon_nam(mtmp));
					mtmp->mtrapped = TRUE;
				}
				/* Possibly vanish */
				if(mtmp->mvanishes>-1){
					if(--mtmp->mvanishes == 0){
						monvanished(mtmp);
						continue;
					}
				}
				if(noactions(mtmp)){
					/* Monsters in a essence trap can't move */
					if(mtmp->mtrapped && t_at(mtmp->mx, mtmp->my) && t_at(mtmp->mx, mtmp->my)->ttyp == VIVI_TRAP){
						mtmp->mhp = 1;
						mtmp->movement = 0;
						continue;
					}
					if(mtmp->entangled){
						if(mtmp->entangled == SHACKLES){
							mtmp->mhp = min(mtmp->mhp, mtmp->mhpmax/2+1);
							mtmp->movement = 0;
							continue;
						}
						else if(mtmp->entangled == MUMMY_WRAPPING){
							if(which_armor(mtmp, W_ARMC) && which_armor(mtmp, W_ARMC)->otyp == MUMMY_WRAPPING){
								if(canseemon(mtmp)) pline("%s struggles against %s wrappings", Monnam(mtmp), hisherits(mtmp));
								mtmp->movement = 0;
								continue;
							} else {
								mtmp->entangled = 0;
							}
						}
						else if(!mbreak_entanglement(mtmp)){
							mtmp->movement = 0;
							mescape_entanglement(mtmp);
							continue;
						}
					}
				}
				/* Possibly wake up */
				if(mtmp->mtame && mtmp->mstrategy&STRAT_WAITMASK){
					mtmp->mstrategy &= ~STRAT_WAITMASK;
				}
				/* Possibly become hostile */
				if(mtmp->mpeacetime && !mtmp->mtame){
					mtmp->mpeacetime--;
					if(!mtmp->mpeacetime) mtmp->mpeaceful = FALSE;
				}
				/* Possibly change hostility */
				if(mtmp->mtyp == PM_SURYA_DEVA){
					struct monst *blade;
					for(blade = fmon; blade; blade = blade->nmon)
						if(blade->mtyp == PM_DANCING_BLADE && mtmp->m_id == blade->mvar_suryaID) break;
					if(blade){
						if(mtmp->mtame && !blade->mtame){
							if(blade == nxtmon) nxtmon = nxtmon->nmon;
							tamedog(blade, (struct obj *) 0);
						} else if(!mtmp->mtame && blade->mtame){
							blade->mtame = 0;
						}
						if(mtmp->mpeaceful != blade->mpeaceful){
							mtmp->mpeaceful = blade->mpeaceful;
						}
					}
				}
				
				/*Reset fracture flag*/
				if(mtmp->zombify && is_kamerel(mtmp->data)) mtmp->zombify = 0;
				
				if(mtmp->mtyp == PM_ARA_KAMEREL) flags.goldka_level=1;
				if(mtmp->mtyp == PM_ASPECT_OF_THE_SILENCE){
					flags.silence_level=1;
					losepw(3);
				}
				if(mtmp->mtyp == PM_ZUGGTMOY) flags.spore_level=1;
				if(mtmp->mtyp == PM_JUIBLEX) flags.slime_level=1;
				if(mtmp->mtyp == PM_PALE_NIGHT || mtmp->mtyp == PM_DREAD_SERAPH || mtmp->mtyp == PM_LEGION) flags.walky_level=1;
				if(mtmp->mtyp == PM_ORCUS || mtmp->mtyp == PM_NAZGUL) flags.shade_level=1;
				if(mtmp->mtyp == PM_DREAD_SERAPH && (mtmp->mstrategy & STRAT_WAITMASK) && (u.uevent.invoked || (Role_if(PM_ANACHRONONAUT) && In_quest(&u.uz)))){
					mtmp->mstrategy &= ~STRAT_WAITMASK;
					pline_The("entire %s is shaking around you!",
						   In_endgame(&u.uz) ? "plane" : "dungeon");
					do_earthquake(mtmp->mx, mtmp->my, min(((int)mtmp->m_lev - 1) / 3 + 1,24), 3, TRUE, mtmp);
					if(Role_if(PM_ANACHRONONAUT) && In_quest(&u.uz)){
						digcrater(mtmp);
					} else if(Is_lolth_level(&u.uz)){
						digcloudcrater(mtmp);
					} else if(rn2(2)){ //Do for x
						digXchasm(mtmp);
					} else { //Do for y
						digYchasm(mtmp);
					}
				}
				
				if(mtmp->mtyp == PM_DEEPEST_ONE && !mtmp->female && u.uevent.ukilled_dagon){
					set_mon_data(mtmp, PM_FATHER_DAGON);
					u.uevent.ukilled_dagon = 0;
				}
				if(mtmp->mtyp == PM_DEEPEST_ONE && mtmp->female && u.uevent.ukilled_hydra){
					set_mon_data(mtmp, PM_MOTHER_HYDRA);
					u.uevent.ukilled_hydra = 0;
				}
				if(mtmp->mtyp == PM_GOLD_GOLEM){
					int golds = u.goldkamcount_tame + level.flags.goldkamcount_peace + level.flags.goldkamcount_hostile;
					if(golds > 0){
						if(canseemon_eyes(mtmp)){
							pline("%s blossoms into a swirl of mirrored arcs!", Monnam(mtmp));
							You("see the image of %s reflected in the golden mirrors!", an(mons[PM_ARA_KAMEREL].mname));
						}
						set_mon_data(mtmp, PM_ARA_KAMEREL);
						mtmp->m_lev = 15;
						mtmp->mhpmax = d(15, 8);
						mtmp->mhp = mtmp->mhpmax;
						if(mtmp->mnamelth) mtmp = christen_monst(mtmp, ""); //Now a different entity
						mtmp->movement = 9;//Don't pause for a turn
						golds = rnd(golds);
						
						golds -= u.goldkamcount_tame;
						if(golds <= 0){
							u.goldkamcount_tame--;
							if(!mtmp->mtame)
								mtmp = tamedog(mtmp, (struct obj *) 0);
							newsym(mtmp->mx,mtmp->my);
							if(!mtmp)
								continue; //something went wrong, and now mtmp is no good
							goto karemade;
						}
						golds -= level.flags.goldkamcount_peace;
						if(golds <= 0){
							level.flags.goldkamcount_peace--;
							mtmp->mtame = 0;
							mtmp->mpeaceful = 1;
							newsym(mtmp->mx,mtmp->my);
							goto karemade;
						}
						level.flags.goldkamcount_hostile--;
						mtmp->mtame = 0;
						mtmp->mpeaceful = 0;
						newsym(mtmp->mx,mtmp->my);
karemade:						
						set_malign(mtmp);
					}
				}
				
				if(mtmp->mtyp == PM_WALKING_DELIRIUM && canseemon(mtmp) && distmin(mtmp->mx, mtmp->my, u.ux, u.uy) <= 1){
					static long lastusedmove = 0;
					if(lastusedmove != moves){
						pline("%s takes on forms new and terrible!", Monnam(mtmp));
						lastusedmove = moves;
						change_usanity(u_sanity_loss(mtmp)/2-1);
					}
				}
				
				if(mtmp->mtyp == PM_DREADBLOSSOM_SWARM){
					if(canseemon(mtmp) || u.ustuck == mtmp) mtmp->movement += mcalcmove(mtmp);
					else {
						struct monst *tmpm;
						for(tmpm = fmon; tmpm; tmpm = tmpm->nmon){
							if(!(is_fey(tmpm->data) || is_plant(tmpm->data)) && mon_can_see_mon(tmpm,mtmp)){
								mtmp->movement += mcalcmove(mtmp);
								break;
							}
						}
					}
				} else mtmp->movement += mcalcmove(mtmp);
				
				if(mtmp->mtyp == PM_NITOCRIS){
					if(which_armor(mtmp, W_ARMC) && which_armor(mtmp, W_ARMC)->oartifact == ART_PRAYER_WARDED_WRAPPINGS_OF)
						mtmp->mspec_used = 1;
				}
				
				if(mtmp->moccupation && !occupation){
					mtmp->moccupation = 0;
					mtmp->mcanmove = 1;
				}
				if(!mtmp->mnotlaugh){
					if(!is_silent_mon(mtmp)){
						wake_nearto_noisy(mtmp->mx, mtmp->my, combatNoise(mtmp->data));
						if(sensemon(mtmp) || (canseemon(mtmp) && !mtmp->mundetected)){
							pline("%s is laughing hysterically.", Monnam(mtmp));
						} else if(couldsee(mtmp->mx,mtmp->my)){
							You_hear("hysterical laughter.");
						} else {
							You_hear("laughter in the distance.");
						}
					} else if(sensemon(mtmp) || (canseemon(mtmp) && !mtmp->mundetected))
						pline("%s is trembling hysterically.", Monnam(mtmp));
				}
//ifdef BARD
				if (mtmp->encouraged && (!rn2(4))) {
					if(mtmp->encouraged > 0) mtmp->encouraged--;
					else mtmp->encouraged++;
					if (!(mtmp->encouraged) && canseemon(mtmp) && mtmp->mtame) 
						pline("%s looks calm again.", Monnam(mtmp));
				}
//endif
				if(mtmp->mtyp == PM_GREAT_CTHULHU || mtmp->mtyp == PM_ZUGGTMOY 
					|| mtmp->mtyp == PM_SWAMP_FERN) mtmp->mspec_used = 0;
				if(is_weeping(mtmp->data)) mtmp->mspec_used = 0;
				if(mtmp->mtyp == PM_CLOCKWORK_SOLDIER || mtmp->mtyp == PM_CLOCKWORK_DWARF || 
				   mtmp->mtyp == PM_FABERGE_SPHERE || mtmp->mtyp == PM_FIREWORK_CART ||
				   mtmp->mtyp == PM_ID_JUGGERNAUT
				) if(rn2(2)) mtmp->mvar_vector = ((int)mtmp->mvar_vector + rn2(3)-1)%8;
				if((mtmp->mtyp == PM_JUGGERNAUT || mtmp->mtyp == PM_ID_JUGGERNAUT) && !rn2(3)){
					int mdx=0, mdy=0, i;
					if(mtmp->mux == 0 && mtmp->muy == 0){
						i = rn2(8);
						mdx = xdir[i];
						mdy = ydir[i];
					} else {
						if(mtmp->mux - mtmp->mx < 0) mdx = -1;
						else if(mtmp->mux - mtmp->mx > 0) mdx = +1;
						if(mtmp->muy - mtmp->my < 0) mdy = -1;
						else if(mtmp->muy - mtmp->my > 0) mdy = +1;
						for(i=0;i<8;i++) if(xdir[i] == mdx && ydir[i] == mdy) break;
					}
					if(mtmp->mvar_vector != i){
						if(sensemon(mtmp) || (canseemon(mtmp) && !mtmp->mundetected)){
							pline("%s turns to a new heading.", Monnam(mtmp));
						} else if(couldsee(mtmp->mx,mtmp->my)){
							You_hear("a loud scraping noise.");
						} else {
							You_hear("scraping in the distance.");
						}
						mtmp->mvar_vector = i;
						mtmp->movement = -12;
					}
				}
			} /* movement rations */
			
			static boolean LBbreach = FALSE;
			static boolean LBproxim = FALSE;
			static boolean LBperim = FALSE;
			if(Role_if(PM_ANACHRONONAUT) && In_quest(&u.uz) && Is_qstart(&u.uz) && !(quest_status.leader_is_dead)){
				for (mtmp = fmon; mtmp; mtmp = mtmp->nmon) if(!mtmp->mpeaceful && mtmp->mx <= 23) break;
				if(mtmp && !(LBbreach && moves%5)) {
					verbalize("**EMERGENCY ALERT: hostile entities detected within Last Bastion**");
					LBbreach = TRUE;
					(void) makemon((struct permonst *)0, xdnstair, ydnstair, MM_ADJACENTOK);
				} else if(!mtmp) LBbreach = FALSE;
				
				for (mtmp = fmon; mtmp; mtmp = mtmp->nmon) if(!mtmp->mpeaceful && mtmp->mx <= 26 && mtmp->mx > 23) break;
				if(mtmp && !LBbreach && !(LBproxim && moves%10)) {
					verbalize("*PROXIMITY ALERT: hostile entities detected outside Last Bastion*");
					LBproxim = TRUE;
				} else if(!mtmp) LBproxim = FALSE;
				
				for (mtmp = fmon; mtmp; mtmp = mtmp->nmon) if(!mtmp->mpeaceful && mtmp->mx <= 55 && mtmp->mx > 26) break;
				if(mtmp && !LBperim) {
					if(mtmp) verbalize("*PERIMETER ALERT: hostile entities closing on Last Bastion*");
					LBperim = TRUE;
				} else if(!mtmp) LBperim = FALSE;
			}
			//Random monster generation block
			if(In_mithardir_terminus(&u.uz) &&
				mvitals[PM_ASPECT_OF_THE_SILENCE].born == 0 &&
				((u.ufirst_light && u.ufirst_sky)
				|| (u.ufirst_light && u.ufirst_life)
				|| (u.ufirst_sky && u.ufirst_life)
			)){
				makemon(&mons[PM_ASPECT_OF_THE_SILENCE], 0, 0, NO_MM_FLAGS);
			}
		    else if(!(Is_illregrd(&u.uz) && u.ualign.type == A_LAWFUL && !u.uevent.uaxus_foe) && /*Turn off random generation on axus's level if lawful*/
				!rn2(u.uevent.udemigod ? 25 :
				(Role_if(PM_ANACHRONONAUT) && In_quest(&u.uz) && !(Is_qstart(&u.uz) && !(quest_status.leader_is_dead))) ? 35 :
			    (depth(&u.uz) > depth(&stronghold_level)) ? 50 : 70)
			){
				if(Is_ford_level(&u.uz)){
					if(rn2(2)){
						int x, y, tries = 200;
						do x = rn2(COLNO/2) + COLNO/2 + 1, y =  rn2(ROWNO-2)+1;
						while((!isok(x,y) || !(levl[x][y].typ == SOIL || levl[x][y].typ == ROOM)) && tries-- > 0);
						if(tries >= 0)
							makemon(ford_montype(1), x, y, MM_ADJACENTOK);
					} else {
						int x, y, tries = 200;
						do x = rn2(COLNO/2) + 1, y =  rn2(ROWNO-2)+1;
						while((!isok(x,y) || !(levl[x][y].typ == SOIL || levl[x][y].typ == ROOM)) && tries-- > 0);
						if(tries >= 0)
							makemon(ford_montype(-1), x, y, MM_ADJACENTOK);
					}
				} else if(!(mvitals[PM_HOUND_OF_TINDALOS].mvflags&G_GONE && !In_quest(&u.uz)) && (level_difficulty()+u.ulevel)/2+5 > monstr[PM_HOUND_OF_TINDALOS] && u.uinsight > rn2(INSIGHT_RATE)){
					int x, y;
					for(x = 1; x < COLNO; x++)
						for(y = 0; y < ROWNO; y++){
							if(IS_CORNER(levl[x][y].typ) && couldsee(x, y) && rn2(45) < u.ulevel){
								flags.cth_attk=TRUE;//state machine stuff.
								create_gas_cloud(x, y, 4, 30);
								flags.cth_attk=FALSE;
								makemon(&mons[PM_HOUND_OF_TINDALOS], x, y, 0);
							}
						}
				} else {
					if (u.uevent.invoked && xupstair && rn2(10)) {
						(void) makemon((struct permonst *)0, xupstair, yupstair, MM_ADJACENTOK);
					} //TEAM ATTACKS
					if(In_sokoban(&u.uz)){
						if(u.uz.dlevel != 1 && u.uz.dlevel != 4) makemon((struct permonst *)0, xupstair, yupstair, MM_ADJACENTSTRICT|MM_ADJACENTOK);
					} else if(Role_if(PM_ANACHRONONAUT) && In_quest(&u.uz) && Is_qstart(&u.uz) && !(quest_status.leader_is_dead)){
						(void) makemon((struct permonst *)0, xdnstair, ydnstair, MM_ADJACENTOK);
						(void) makemon((struct permonst *)0, xdnstair, ydnstair, MM_ADJACENTOK);
						(void) makemon((struct permonst *)0, xdnstair, ydnstair, MM_ADJACENTOK);
						(void) makemon((struct permonst *)0, xdnstair, ydnstair, MM_ADJACENTOK);
					}
					else (void) makemon((struct permonst *)0, 0, 0, NO_MM_FLAGS);
				}
			}
			if(Role_if(PM_ANACHRONONAUT) && In_quest(&u.uz) && !(Is_qstart(&u.uz) && !Race_if(PM_ANDROID)) && !rn2(35)){
				struct monst* mtmp = makemon(&mons[PM_SEMBLANCE], rn1(COLNO-3,2), rn1(ROWNO-3,2), MM_ADJACENTOK);
				//"Where stray illuminations from the Far Realm leak onto another plane, matter stirs at the beckoning of inexplicable urges before burning to ash."
				if(mtmp && canseemon(mtmp)) pline("The base matter of the world stirs at the beckoning of inexplicable urges, dancing with a semblance of life.");
			}

		    /* reset summon monster block. */
			if(u.summonMonster) u.summonMonster = FALSE;

		    /* calculate how much time passed. */
#ifdef STEED
		    if (u.usteed && u.umoved) {
			/* your speed doesn't augment steed's speed */
			moveamt = mcalcmove(u.usteed);
			moveamt += P_SKILL(P_RIDING)-1; /* -1 to +3 */
			if(uclockwork){
				if(u.ucspeed == HIGH_CLOCKSPEED){
					/*You are still burning spring tension, even if it doesn't affect your speed!*/
					if(u.slowclock < 3) morehungry(3-u.slowclock);
					else if(!(moves%(u.slowclock - 2))) morehungry(1);
				}
				if(u.phasengn){
					//Phasing mount as well
					morehungry(10);
				}
			}
		    } else
#endif
		    {
			moveamt = youmonst.data->mmove;
			if(!Upolyd && Race_if(PM_HALF_DRAGON) && !(Role_if(PM_NOBLEMAN) && flags.initgend)) moveamt = (moveamt*2)/3;
			if(uarmf && uarmf->otyp == STILETTOS && !Flying && !Levitation) moveamt = (moveamt*5)/6;
			
			if(u.sealsActive&SEAL_EURYNOME && IS_PUDDLE_OR_POOL(levl[u.ux][u.uy].typ)){
				if (Very_fast) {	/* speed boots or potion */
					/* average movement is 1.78 times normal */
					moveamt += 2*NORMAL_SPEED / 3;
					if (rn2(3) == 0) moveamt += NORMAL_SPEED / 3;
				} else if (Fast) {
					/* average movement is 1.56 times normal */
					moveamt += NORMAL_SPEED / 3;
					if (rn2(3) != 0) moveamt += NORMAL_SPEED / 3;
				} else {
					/* average movement is 1.33 times normal */
					if (rn2(3) != 0) moveamt += NORMAL_SPEED / 2;
				}
			} else {
				if (Very_fast) {	/* speed boots or potion */
					/* average movement is 1.67 times normal */
					moveamt += NORMAL_SPEED / 2;
					if (rn2(3) == 0) moveamt += NORMAL_SPEED / 2;
				} else if (Fast) {
					/* average movement is 1.33 times normal */
					if (rn2(3) != 0) moveamt += NORMAL_SPEED / 2;
				}
			}
			
			if(uwep && uwep->oartifact == ART_SINGING_SWORD && uwep->osinging == OSING_HASTE){
				moveamt += 2;
			}
			
			if(u.specialSealsActive&SEAL_BLACK_WEB && u.utrap && u.utraptype == TT_WEB)
				moveamt += 8;
			
			if(u.sealsActive&SEAL_ENKI){
				nmonsclose = nmonsnear = 0;
				for (mtmp = fmon; mtmp; mtmp = mtmp->nmon){
					if(mtmp->mpeaceful) continue;
					if(distmin(u.ux, u.uy, mtmp->mx,mtmp->my) <= 1){
						nmonsclose++;
						nmonsnear++;
					} else if(distmin(u.ux, u.uy, mtmp->mx,mtmp->my) <= 2){
						nmonsnear++;
					}
				}
				enkispeedlim = u.ulevel/10+1;
				if(nmonsclose>1){
					moveamt += min(enkispeedlim,nmonsclose);
					nmonsnear -= min(enkispeedlim,nmonsclose);
				}
				if(nmonsnear>3) moveamt += min(enkispeedlim,nmonsnear-2);
			}
			if (uwep && uwep->oartifact == ART_GARNET_ROD) moveamt += NORMAL_SPEED / 2;
			if (uwep && uwep->oartifact == ART_TENSA_ZANGETSU){
				moveamt += NORMAL_SPEED;
				if(u.ZangetsuSafe-- < 1){
					if(ublindf && ublindf->otyp == MASK && is_undead(&mons[ublindf->corpsenm])){
						u.ZangetsuSafe = mons[ublindf->corpsenm].mlevel;
						if(ublindf->ovar1>=3){
							Your("mask shatters!");
							useup(ublindf);
						} else {
							Your("mask cracks.");
							ublindf->ovar1++;
						}
					} else {
						u.ZangetsuSafe = 0;
						losehp(5, "inadvisable haste", KILLED_BY);
						if (Upolyd) {
							if(u.mhmax > u.ulevel && moves % 2){
								u.uhpmod--;
								calc_total_maxhp();
							}
						}
						else{
							if(u.uhpmax > u.ulevel && moves % 2){
								u.uhpmod--;
								calc_total_maxhp();
							}
						}
	//					if( !(moves % 5) )
						You_feel("your %s %s!", 
							body_part(BONES), rnd(6) ? body_part(CREAK) : body_part(CRACK));
						exercise(A_CON, FALSE);
						exercise(A_STR, FALSE);
					}
				}
			}
			else if(u.ZangetsuSafe < u.ulevel && !(moves%10)) u.ZangetsuSafe++;
			
			if(!(moves%10)){
				if(u.eurycounts) u.eurycounts--;
				if(u.orthocounts){
					if(u.nv_range){
						u.orthocounts -= u.nv_range;
						if(u.orthocounts < 0) u.orthocounts = 0;
					} else {
						u.orthocounts--;
					}
				}
				if(u.wimage){
					if(u.wimage >= 10){
						exercise(A_INT, TRUE);
						exercise(A_WIS, FALSE);
					} else if(!(moves%10)) u.wimage--;
				}
				if(u.umorgul){
					int i = A_CON;
					int n = u.umorgul;
					if(ACURR(A_WIS)>=ACURR(i))
						i = A_WIS;
					if(ACURR(A_CHA)>=ACURR(i))
						i = A_CHA;
					while(n-- > 0){
						exercise(i, FALSE);
					}
				}
			}
			
			if(u.sealsActive&SEAL_ASTAROTH && u.uinwater){
				losehp(1, "rusting through", KILLED_BY);
			}
			
			if(uclockwork && u.ucspeed == HIGH_CLOCKSPEED){
				int hungerup;
				moveamt *= 2;
				hungerup = 2*moveamt/NORMAL_SPEED - 1;
				if(u.slowclock < hungerup) morehungry(hungerup-u.slowclock);
				else if(!(moves%(u.slowclock - hungerup + 1))) morehungry(1);
			}
			if(uandroid && u.ucspeed == HIGH_CLOCKSPEED){
				if (rn2(3) != 0) moveamt += NORMAL_SPEED / 2;
			}
			if(active_glyph(ANTI_CLOCKWISE_METAMORPHOSIS))
				moveamt += 3;
			if(u.uuur_duration)
				moveamt += 6;
			if(uwep && is_lightsaber(uwep) && litsaber(uwep) && u.fightingForm == FFORM_SORESU && (!uarm || is_light_armor(uarm) || is_medium_armor(uarm))){
				switch(min(P_SKILL(FFORM_SORESU), P_SKILL(weapon_type(uwep)))){
					case P_BASIC:       moveamt = max(moveamt-6,1); break;
					case P_SKILLED:     moveamt = max(moveamt-4,1); break;
					case P_EXPERT:      moveamt = max(moveamt-3,1); break;
				}
			}
			if(youracedata->mmove){
				if(moveamt < 1) moveamt = 1;
			} else {
				if(moveamt < 0) moveamt = 0;
			}
			}
			
			if(uclockwork && u.phasengn){
				morehungry(10);
			}
			
			if(uclockwork && u.ucspeed == SLOW_CLOCKSPEED)
				moveamt /= 2; /*Even if you are mounted, a slow clockspeed affects how 
								fast you can issue commands to the mount*/
			
		    switch (wtcap) {
			case UNENCUMBERED: break;
			case SLT_ENCUMBER: moveamt -= (moveamt / 4); break;
			case MOD_ENCUMBER: moveamt -= (moveamt / 2); break;
			case HVY_ENCUMBER: moveamt -= ((moveamt * 3) / 4); break;
			case EXT_ENCUMBER: moveamt -= ((moveamt * 7) / 8); break;
			default: break;
		    }
			
			if(u.umadness&MAD_NUDIST && !ClearThoughts && u.usanity < 100){
				int delta = 100 - u.usanity;
				int discomfort = u_clothing_discomfort();
				discomfort = (discomfort * delta)/100;
				if (moveamt - discomfort < NORMAL_SPEED/2) {
					moveamt = min(moveamt, NORMAL_SPEED/2);
				}
				else moveamt -= discomfort;
			}
			
		    if(In_fog_cloud(&youmonst)) moveamt = max(moveamt/3, 1);
		    youmonst.movement += moveamt;
			//floor how far into movement-debt you can fall.
		    if (youmonst.movement < -2*NORMAL_SPEED) youmonst.movement = -2*NORMAL_SPEED;
			
			//Aprox one check per six monster-gen periods
			if(!rn2(70*6) && roll_madness(MAD_SPORES)
				&& !Race_if(PM_ANDROID) && !Race_if(PM_CLOCKWORK_AUTOMATON)
			){
				make_hallucinated(itimeout_incr(HHallucination, 100), TRUE, 0L);
				if(roll_madness(MAD_SPORES)){//Second roll for more severe symptoms
					make_stunned(itimeout_incr(HStun, 100), TRUE);
					make_confused(itimeout_incr(HConfusion, 100), FALSE);
					losehp(u.ulevel, "brain-eating fungal spores", KILLED_BY);
					exercise(A_INT, FALSE);exercise(A_WIS, FALSE);exercise(A_CHA, FALSE);
					exercise(A_INT, FALSE);exercise(A_WIS, FALSE);exercise(A_CHA, FALSE);
					exercise(A_INT, FALSE);exercise(A_WIS, FALSE);exercise(A_CHA, FALSE);
				}
				else {
					make_confused(itimeout_incr(HConfusion, 100), TRUE);
					losehp(u.ulevel/2, "brain-eating fungal spores", KILLED_BY);
					exercise(A_INT, FALSE);
					exercise(A_WIS, FALSE);
					exercise(A_CHA, FALSE);
				}
			}
			if(!rn2(8) && !flaming(youracedata) && roll_madness(MAD_COLD_NIGHT)){
				if(u.usanity <= 88 && !rn2(11)){
					if(!Breathless && !Blind)
						Your("breath crystallizes in the airless void!");
					FrozenAir += (100-u.usanity)/12;
					delayed_killer = "the cold night";
					nomul(0, NULL);
				}
				roll_av_frigophobia();
				if(!InvCold_resistance || !rn2(11)){
					destroy_item(POTION_CLASS, AD_COLD);
				}
			} else if(FrozenAir){
				roll_av_frigophobia();
				if(!InvCold_resistance || !rn2(11)){
					destroy_item(POTION_CLASS, AD_COLD);
				}
			}
			
			if(!rn2(9) && roll_madness(MAD_OVERLORD)){
				You("feel its burning gaze upon you!");
				u.ustdy += 9;
				if(!InvFire_resistance || !rn2(11)){
					destroy_item(POTION_CLASS, AD_FIRE);
					destroy_item(SCROLL_CLASS, AD_FIRE);
				}
				if(!rn2(11) && roll_madness(MAD_OVERLORD)){
					u.ustdy += 90;
					destroy_item(SPBOOK_CLASS, AD_FIRE);
				}
				nomul(0, NULL);
			}
			
			//Aprox one check per five monster-gen periods, or one per five while sleeping (averages one additional blast per sleep, so it's really bad.
			if((u.usleep || !rn2(70)) && !rn2(5) && roll_madness(MAD_DREAMS)){
				cthulhu_mind_blast();
			}
			
			if(u.uentangled){ //Note: the normal speed calculations include important hunger modifiers, so just calculate speed then 0 it out if needed.
				if(!ubreak_entanglement()){
					youmonst.movement = 0;
					if(u.uentangled == RAZOR_WIRE){
						int dmg = d(1,6);
						int beat;
						if(hates_silver(youracedata) && entangle_material(&youmonst, SILVER))
							dmg += rnd(20);
						if(hates_iron(youracedata) && entangle_material(&youmonst, SILVER))
							dmg += rnd(u.ulevel);
						beat = entangle_beatitude(&youmonst, -1);
						if(hates_unholy(youracedata) && beat)
							dmg += beat == 2 ? d(2,9) : rnd(9);
						beat = entangle_beatitude(&youmonst, 1);
						if(hates_holy(youracedata) && beat)
							dmg += beat == 2 ? rnd(20) : rnd(4);
						losehp(dmg, "being sliced to ribbons by razor wire", KILLED_BY);
					}
					uescape_entanglement();
				}
			}
			
		    settrack();

		    monstermoves++;
		    moves++;
			nonce = rand();

		      /********************************/
		     /* once-per-turn things go here */
		    /********************************/
			/* Environment effects */
			dust_storm();
			/* Unseen monsters may take action */
			for(mtmp = migrating_mons; mtmp; mtmp = nxtmon){
				nxtmon = mtmp->nmon;
				unseen_actions(mtmp); //May cause mtmp to be removed from the migrating chain
			}
			
			/* Item attacks */
			mind_blast_items();
			if(uarm && 
				(uarm->otyp == LIVING_ARMOR || uarm->otyp == BARNACLE_ARMOR)
			) dosymbiotic();
			if(u.spiritPColdowns[PWR_PSEUDONATURAL_SURGE] >= moves+20)
				dopseudonatural();
			if(roll_madness(MAD_GOAT_RIDDEN) && adjacent_mon()){
				pline("Lashing tentacles erupt from your brain!");
				losehp(max(1,(Upolyd ? ((d(4,4)*u.mh)/u.mhmax) : ((d(4,4)*u.uhp)/u.uhpmax))), "the black mother's touch", KILLED_BY);
				morehungry(d(4,4));
				if(u.usanity < 50)
					change_usanity(-1);
				else
					change_usanity(-1*d(4,4));
				dogoat();
			}
			if(Destruction)
				dodestruction();
			if(Mindblasting)
				domindblast_strong();
			/* Clouds on Lolth's level deal damage */
			if(Is_lolth_level(&u.uz) && levl[u.ux][u.uy].typ == CLOUD){
				if (!(nonliving(youracedata) || Breathless)){
					if (!Blind)
						make_blinded((long)rnd(4), FALSE);
					if (!Poison_resistance) {
						pline("%s is burning your %s!", Something, makeplural(body_part(LUNG)));
						You("cough and spit blood!");
						losehp(d(3,8) + ((Amphibious && !flaming(youracedata)) ? 0 : rnd(6)), "stinking cloud", KILLED_BY_AN);
					} else if (!(Amphibious && !flaming(youracedata))){
						You("are laden with moisture and %s",
							flaming(youracedata) ? "are smoldering out!" :
							Breathless ? "find it mildly uncomfortable." :
							amphibious(youracedata) ? "feel comforted." :
							"can barely breathe!");
						/* NB: Amphibious includes Breathless */
						if (!(Amphibious && !flaming(youracedata))) losehp(rnd(6), "suffocating in a cloud", KILLED_BY);
					} else {
						You("cough!");
					}
				} else {
					You("are laden with moisture and %s",
						flaming(youracedata) ? "are smoldering out!" :
						Breathless ? "find it mildly uncomfortable." :
						amphibious(youracedata) ? "feel comforted." :
						"can barely breathe!");
					/* NB: Amphibious includes Breathless */
					if (!(Amphibious && !flaming(youracedata))) losehp(rnd(6), "suffocating in a cloud", KILLED_BY);
				}
			}
			/* If the player has too many pets, untame them untill that is no longer the case */
			{
				// finds weakest pet, and if there's more than 6 pets that count towards your limit
				// it sets the weakest one friendly - dog.c
				enough_dogs(0);
				
				// if there's a spiritual pet that isn't already marked for vanishing,
				// give it 5 turns before it disappears. - dog.c
				vanish_dogs();
			}
			
			if(u.petattacked){
				u.petattacked = FALSE;
				use_skill(P_BEAST_MASTERY, 1);
			}
			if(u.pethped){
				u.pethped = FALSE;
				more_experienced(u.ulevel,0);
				newexplevel();
			}
			if (u.uencouraged && (!rn2(4))) {
				if(u.uencouraged > 0) u.uencouraged--;
				else u.uencouraged++;
				if (!(u.uencouraged)) 
					You_feel("calm again.");
			}
			
		    if (flags.bypasses) clear_bypasses();
		    if(Glib) glibr();
		    nh_timeout();
		    run_regions();
		    run_maintained_spells();
			
			move_gliders();

		    if (u.ublesscnt)  u.ublesscnt--;
		    if (u.ugoatblesscnt && u.uevent.shubbie_atten && !u.ugangr[GA_MOTHER])
				u.ugoatblesscnt--;
		    if(flags.time && !flags.run)
			flags.botl = 1;
			
		    if ((prev_dgl_extrainfo == 0) || (prev_dgl_extrainfo < (moves + 250))) {
				prev_dgl_extrainfo = moves;
				mk_dgl_extrainfo();
		    }
			if(uclockwork){
				if(u.ustove){
					if(u.uboiler){
						int steam = min(10,min(u.ustove,u.uboiler));
						lesshungry(steam);
						u.ustove -= min(10,u.ustove);
						u.uboiler-=steam;
						flags.cth_attk=TRUE;//state machine stuff.
						create_gas_cloud(u.ux+rn2(3)-1, u.uy+rn2(3)-1, 1, rnd(3));
						flags.cth_attk=FALSE;
						if(u.utemp && moves%2) u.utemp--;
					} else {
						if(u.utemp<=WARM || 
							(u.utemp<=HOT && (moves%4)) ||
							(u.utemp>HOT && u.utemp<=BURNING_HOT && (moves%3)) ||
							(u.utemp>BURNING_HOT && u.utemp<=MELTING && (moves%2)) ||
							(u.utemp>MELTING && u.utemp<=MELTED && !(moves%3)) ||
							(u.utemp>MELTED && !(moves%4))
						) u.utemp++;
						flags.cth_attk=TRUE;//state machine stuff.
						create_gas_cloud(u.ux+rn2(3)-1, u.uy+rn2(3)-1, 1, rnd(6)); //Longer-lived smoke
						flags.cth_attk=FALSE;
						u.ustove -= min(10,u.ustove);
					}
				} else if(u.utemp) u.utemp--;
				if(u.utemp > BURNING_HOT){
					if((u.utemp-5)*2 > rnd(10)) destroy_item(SCROLL_CLASS, AD_FIRE);
					if((u.utemp-5)*2 > rnd(10)) destroy_item(POTION_CLASS, AD_FIRE);
					if((u.utemp-5)*2 > rnd(10)) destroy_item(SPBOOK_CLASS, AD_FIRE);
					
					if(u.utemp >= MELTING && !(HFire_resistance || u.sealsActive&SEAL_FAFNIR)){
						You("are melting!");
						losehp(u.ulevel, "melting from extreme heat", KILLED_BY);
						if(u.utemp >= MELTED){
							if(Upolyd) losehp(u.mhmax*2, "melting to slag", KILLED_BY);
							else { /* May have been rehumanized by previous damage. In that case, still die from left over bronze on your skin! */
								if(uclockwork) losehp((Upolyd ? u.mhmax : u.uhpmax)*2, "melting to slag", KILLED_BY);
								else if(!(HFire_resistance || u.sealsActive&SEAL_FAFNIR)) losehp((Upolyd ? u.mhmax : u.uhpmax)*2, "molten bronze", KILLED_BY);
							}
						}
					}
				}
			} else if(u.utemp) u.utemp = 0;
			
			if(u.uinwater && !u.umoved){
				if(uclockwork) u.uboiler = MAX_BOILER;
			}
			
			if(u.ukinghill){
				if(u.protean > 0) u.protean--;
				else{
					for(pobj = invent; pobj; pobj=pobj->nobj)
						if(pobj->oartifact == ART_TREASURY_OF_PROTEUS)
							break;
					if(!pobj){
						pline("Treasury not actually in inventory??");
						u.ukinghill = 0;
					}
					else if(pobj->cobj){
						arti_poly_contents(pobj);
					}
					u.protean = 100 + d(10,10);
					update_inventory();
				}
			}

			// Ymir's stat regeneration
			if (u.sealsActive&SEAL_YMIR && (wtcap < MOD_ENCUMBER || !u.umoved || Regeneration)){
				if ((u.ulevel > 9 && !(moves % 3)) ||
					(u.ulevel <= 9 && !(moves % ((MAXULEV + 12) / (u.ulevel + 2) + 1)))
					){
					int val_limit, idx;
					for (idx = 0; idx < A_MAX; idx++) {
						val_limit = AMAX(idx);
						/* don't recover strength lost from hunger */
						if (idx == A_STR && u.uhs >= WEAK) val_limit--;

						if (val_limit > ABASE(idx)) ABASE(idx)++;
					}
				}
			}

		    /* One possible result of prayer is healing.  Whether or
		     * not you get healed depends on your current hit points.
		     * If you are allowed to regenerate during the prayer, the
		     * end-of-prayer calculation messes up on this.
		     * Another possible result is rehumanization, which requires
		     * that encumbrance and movement rate be recalculated.
		     */
			you_regen_hp();
			you_regen_pw();
			androidUpkeep();

		    if(!(u.uinvulnerable || u.spiritPColdowns[PWR_PHASE_STEP] >= moves+20)) {
			if(Teleportation && !rn2(85) && !(
#ifdef WIZARD
				(
#endif
				 (u.uhave.amulet || On_W_tower_level(&u.uz)
#ifdef STEED
				  || (u.usteed && mon_has_amulet(u.usteed))
#endif
				 )
#ifdef WIZARD
				 && (!wizard) )
#endif
			)) {
			    xchar old_ux = u.ux, old_uy = u.uy;
			    tele();
			    if (u.ux != old_ux || u.uy != old_uy) {
				if (!next_to_u()) {
				    check_leash(old_ux, old_uy);
				}
#ifdef REDO
				/* clear doagain keystrokes */
				pushch(0);
				savech(0);
#endif
			    }
			}
			/* delayed change may not be valid anymore */
			if ((change == 1 && !Polymorph) ||
			    (change == 2 && u.ulycn == NON_PM))
			    change = 0;
			if(Polymorph && !rn2(100))
			    change = 1;
			else if (u.ulycn >= LOW_PM && !Upolyd &&
				 !umechanoid&&
				 !rn2(80 - (20 * night())))
			    change = 2;
			if (change && !Unchanging) {
			    if (multi >= 0) {
				if (occupation)
				    stop_occupation();
				else
				    nomul(0, NULL);
				if (change == 1) polyself(FALSE);
				else you_were();
				change = 0;
			    }
			}
		    }

		    if(Searching && multi >= 0) (void) dosearch0(1);
		    dosounds();
		    do_storms();
		    gethungry();
		    age_spells();
		    exerchk();
		    invault();
		    if (u.uhave.amulet) amulet();
		    if (!rn2(40+(int)(ACURR(A_DEX)*3)))
			u_wipe_engr(rnd(3));
		    if (u.uevent.udemigod && !(u.uinvulnerable || u.spiritPColdowns[PWR_PHASE_STEP] >= moves+20)) {
				if (u.udg_cnt) u.udg_cnt--;
				if (!u.udg_cnt) {
					intervene();
					u.udg_cnt = rn1(200, 50);
				}
		    }
		    if (u.uevent.ukilled_illurien && !(u.uinvulnerable || u.spiritPColdowns[PWR_PHASE_STEP] >= moves+20)) {
				if (u.ill_cnt) u.ill_cnt--;
				if (!u.ill_cnt) {
					illur_intervene();
					u.ill_cnt = rn1(1000, 250);
				}
		    }
		    restore_attrib();
		    /* underwater and waterlevel vision are done here */
		    if (Is_waterlevel(&u.uz))
			movebubbles();
		    else if (Underwater)
			under_water(0);
		    /* vision while buried done here */
		    else if (u.uburied) under_ground(0);

		    /* when immobile, count is in turns */
		    if(multi < 0) {
			if (++multi == 0) {	/* finished yet? */
			    unmul((char *)0);
			    /* if unmul caused a level change, take it now */
			    if (u.utotype) deferred_goto();
			}
		    }
		}
	    } while (youmonst.movement<NORMAL_SPEED); /* hero can't move loop */

	      /******************************************/
	     /* once-per-hero-took-time things go here */
	    /******************************************/
		
		if(u.ustdy > 0) u.ustdy -= 1;
		
		if(u.specialSealsActive&SEAL_LIVING_CRYSTAL)
			average_dogs();
		
		//Mithardir portals
		if(In_mithardir_desert(&u.uz)){
			struct trap *ttmp;
			for(ttmp = ftrap; ttmp; ttmp = ttmp->ntrap) {
				if(ttmp->ttyp == MAGIC_PORTAL && distmin(u.ux, u.uy, ttmp->tx, ttmp->ty) < 3){
					if(!ttmp->tseen){
						ttmp->tseen = 1;
						newsym(ttmp->tx, ttmp->ty);
						if(cansee(ttmp->tx, ttmp->ty)){
							You("spot a swirling portal!");
						} else {
							You_hear("swirling dust!");
						}
					}
				}
			}
		}
		
		//Echolocation
		if(Echolocation){
			for(i=1; i<COLNO; i++)
				for(j=0; j<ROWNO; j++)
					if(viz_array[j][i]&COULD_SEE)
						echo_location(i, j);
			see_monsters();
		}
		if(u.utrap && u.utraptype == TT_LAVA) {
			if(!is_lava(u.ux,u.uy))
			    u.utrap = 0;
		    else if (!(u.uinvulnerable || u.spiritPColdowns[PWR_PHASE_STEP] >= moves+20)) {
			    u.utrap -= 1<<8;
				if(u.utrap < 1<<8) {
					killer_format = KILLED_BY;
					killer = "molten lava";
					You("sink below the surface and die.");
					done(DISSOLVED);
				} else if(didmove && !u.umoved) {
					Norep("You sink deeper into the lava.");
					u.utrap += rnd(4);
				}
			}
	    }
		see_nearby_monsters();
		if(u.uinwater){//Moving around will also call this, so your stuff degrades faster if you move
			water_damage(invent, FALSE, FALSE, level.flags.lethe, &youmonst);
		}

		hpDiff -= u.uhp;
		hpDiff = (hpDiff > 0) ? hpDiff : 0;
		if(uarmg && ART_GAUNTLETS_OF_THE_BERSERKER == uarmg->oartifact){
			float a = .1; /* closer to 1 -> discard older faster */
			long next = (long)(a * hpDiff + (1 - a) * uarmg->ovar1);
			next = (next > 10) ? 10 : next;
			long diff = next - uarmg->ovar1;
			uarmg->ovar1 = next;
			//if(diff) adj_abon(uarmg, diff);
		}
		didmove = FALSE;
	} /* actual time passed */

	/****************************************/
	/* once-per-player-input things go here */
	/****************************************/
	find_ac();
////////////////////////////////////////////////////////////////////////////////////////////////
	for (mtmp = fmon; mtmp; mtmp = nxtmon){
		nxtmon = mtmp->nmon;
		if(mtmp->mtyp == PM_HELLCAT){
			if(!isdark(mtmp->mx,mtmp->my) && !mtmp->minvis){
				mtmp->minvis = TRUE;
				mtmp->perminvis = TRUE;
				newsym(mtmp->mx,mtmp->my);
			} else if(isdark(mtmp->mx,mtmp->my) && mtmp->minvis){
				mtmp->minvis = FALSE;
				mtmp->perminvis = FALSE;
				newsym(mtmp->mx,mtmp->my);
			}
		} else if(mtmp->mtyp == PM_HUDOR_KAMEREL){
			if(is_pool(mtmp->mx,mtmp->my, TRUE) && !mtmp->minvis){
				mtmp->minvis = TRUE;
				mtmp->perminvis = TRUE;
				newsym(mtmp->mx,mtmp->my);
			} else if(!is_pool(mtmp->mx,mtmp->my, TRUE) && mtmp->minvis){
				mtmp->minvis = FALSE;
				mtmp->perminvis = FALSE;
				newsym(mtmp->mx,mtmp->my);
			}
		} else if(mtmp->mtyp == PM_GRUE || mtmp->mtyp == PM_INVIDIAK){
			if(isdark(mtmp->mx,mtmp->my) && !mtmp->minvis){
				mtmp->minvis = TRUE;
				mtmp->perminvis = TRUE;
				newsym(mtmp->mx,mtmp->my);
			} else if(!isdark(mtmp->mx,mtmp->my) && mtmp->minvis){
				mtmp->minvis = FALSE;
				mtmp->perminvis = FALSE;
				newsym(mtmp->mx,mtmp->my);
			}
		}
		if(mtmp->m_insight_level > u.uinsight){
			insight_vanish(mtmp);
			continue;
		}
		if(mtmp->mfaction == DELOUSED){
			delouse_tame(mtmp);
			continue;
		}
		if (mtmp->minvis){
			newsym(mtmp->mx, mtmp->my);
		}
		if (Blind && Bloodsense && has_blood_mon(mtmp)){
			newsym(mtmp->mx, mtmp->my);
		}
	}
////////////////////////////////////////////////////////////////////////////////////////////////
	if(!flags.mv || Blind || oldBlind != (!!Blind)) {
	    /* redo monsters if hallu or wearing a helm of telepathy */
	    if (Hallucination) {	/* update screen randomly */
			see_monsters();
			see_objects();
			see_traps();
			if (u.uswallow) swallowed(0);
		} else if (Unblind_telepat || goodsmeller(youracedata) || Warning || Warn_of_mon || u.usanity < 100 || oldsanity < 100) {
	     	see_monsters();
	    }
		
		oldsanity = u.usanity;

		switch (((u_healing_penalty() - healing_penalty) > 0) - ((u_healing_penalty() - healing_penalty) < 0))
		{
		case 0:
			break;
		case 1:
			if (!Hallucination){
				You_feel("%s.", (healing_penalty) ? "itchier" : "itchy");
			}
			else {
				You_feel("%s.", (healing_penalty) ? "uglier" : "ugly");
			}
			healing_penalty = u_healing_penalty();
			break;
		case -1:
			if (!Hallucination) {
				You_feel("%s.", (u_healing_penalty()) ? "some relief" : "relieved");
			}
			else {
				You_feel("%s.", (u_healing_penalty()) ? "pretty" : "beautiful");
			}
			healing_penalty = u_healing_penalty();
			break;
		}
		
		if (!oldBlind ^ !Blind) {  /* one or the other but not both */
			see_monsters();
			flags.botl = 1;
			vision_full_recalc = 1;	/* blindness just got toggled */
			if (Blind_telepat || Infravision) see_monsters();
		}
		
	    if (vision_full_recalc) vision_recalc(0);	/* vision! */
	}
	oldBlind = !!Blind;
////////////////////////////////////////////////////////////////////////////////////////////////
	if(!flags.mv || LightBlind || oldLightBlind != (!!LightBlind)) {
	    /* redo monsters if hallu or wearing a helm of telepathy */
	    if (Hallucination) {	/* update screen randomly */
			see_monsters();
			see_objects();
			see_traps();
			if (u.uswallow) swallowed(0);
		} else if (Unblind_telepat || goodsmeller(youracedata) || Warning || Warn_of_mon || u.usanity < 100 || oldsanity < 100) {
	     	see_monsters();
	    }
		
		oldsanity = u.usanity;

		if (!oldLightBlind ^ !LightBlind) {  /* one or the other but not both */
			see_monsters();
			flags.botl = 1;
			vision_full_recalc = 1;	/* blindness just got toggled */
			if (Blind_telepat || Infravision) see_monsters();
		}
		
	    if (vision_full_recalc) vision_recalc(0);	/* vision! */
	}
	oldLightBlind = !!LightBlind;
////////////////////////////////////////////////////////////////////////////////////////////////
	calc_total_maxhp();
////////////////////////////////////////////////////////////////////////////////////////////////
	calc_total_maxen();
////////////////////////////////////////////////////////////////////////////////////////////////
	/*If anything we did caused us to get moved out of water, surface*/
	if(u.usubwater && !is_pool(u.ux, u.uy, FALSE)){
		u.usubwater = 0;
		vision_full_recalc = 1;
		vision_recalc(2);	/* unsee old position */
		doredraw();
	} else if(Is_waterlevel(&u.uz) && u.usubwater && !is_3dwater(u.ux, u.uy)){
		You("pop out into an airbubble!");
		u.usubwater = 0;
	} else if(Is_waterlevel(&u.uz) && !u.usubwater && is_3dwater(u.ux, u.uy)){
		Your("%s goes under water!", body_part(HEAD));
		u.usubwater = 1;
	}
////////////////////////////////////////////////////////////////////////////////////////////////
	
	
#ifdef REALTIME_ON_BOTL
	if(iflags.showrealtime) {
		/* Update the bottom line if the number of minutes has
		 * changed */
		if(get_realtime() / 60 != realtime_data.last_displayed_time / 60)
			flags.botl = 1;
	}
#endif
  
	if(flags.botl || flags.botlx) bot();

	if (iflags.hp_notify && (prev_hp_notify != uhp())) {
	  pline("%s", hpnotify_format_str(iflags.hp_notify_fmt ? iflags.hp_notify_fmt : "[HP%c%a=%h]"));
	  prev_hp_notify = uhp();
	}

	flags.move = 1;

	if(multi >= 0 && occupation) {
#if defined(MICRO) || defined(WIN32)
	    abort_lev = 0;
	    if (kbhit()) {
		if ((ch = Getchar()) == ABORT)
		    abort_lev++;
# ifdef REDO
		else
		    pushch(ch);
# endif /* REDO */
	    }
	    if (!abort_lev && (*occupation)() == 0)
#else
	    if ((*occupation)() == 0)
#endif
		occupation = 0;
	    if(
#if defined(MICRO) || defined(WIN32)
		   abort_lev ||
#endif
		   monster_nearby()) {
		stop_occupation();
		reset_eat();
	    }
#if defined(MICRO) || defined(WIN32)
	    if (!(++occtime % 7))
		display_nhwindow(WIN_MAP, FALSE);
#endif
	    continue;
	}

	if ((u.uhave.amulet || Clairvoyant) &&
	    !In_endgame(&u.uz) && !BClairvoyant &&
	    !(moves % 15) && !rn2(2))
		do_vicinity_map(u.ux,u.uy);

#ifdef WIZARD
	if (iflags.sanity_check || iflags.debug_fuzzer)
	    sanity_check();
#endif

#ifdef CLIPPING
	/* just before rhack */
	cliparound(u.ux, u.uy);
#endif

	u.umoved = FALSE;

	if (multi > 0) {
	    lookaround();
	    if (!multi) {
		/* lookaround may clear multi */
		flags.move = 0;
		if (flags.time) flags.botl = 1;
		continue;
	    }
	    if (flags.mv) {
		if(multi < COLNO && !--multi)
		    flags.travel = iflags.travel1 = flags.mv = flags.run = 0;
		domove();
	    } else {
		--multi;
		rhack(save_cm);
	    }
	} else if (multi == 0) {
	  ck_server_admin_msg();
#ifdef MAIL
	    ckmailstatus();
#endif
	    rhack((char *)0);
	}
	if (u.utotype)		/* change dungeon level */
	    deferred_goto();	/* after rhack() */
	/* !flags.move here: multiple movement command stopped */
	else if (flags.time && (!flags.move || !flags.mv))
	    flags.botl = 1;

	if (vision_full_recalc) vision_recalc(0);	/* vision! */
	/* when running in non-tport mode, this gets done through domove() */
	if ((!flags.run || iflags.runmode == RUN_TPORT) &&
		(multi && (!flags.travel ? !(multi % 7) : !(moves % 7L)))) {
	    if (flags.time && flags.run) flags.botl = 1;
	    display_nhwindow(WIN_MAP, FALSE);
	}

	if (moves > last_clear_screen + 2000) doredraw();

    }
}

#endif /* OVL0 */
#ifdef OVL1

void
stop_occupation()
{
	struct monst *mtmp;
	if(occupation) {
		if (!maybe_finished_meal(TRUE))
		    You("stop %s.", occtxt);
		occupation = 0;
		flags.botl = 1; /* in case u.uhs changed */
		for (mtmp = fmon; mtmp; mtmp = mtmp->nmon){
			if(mtmp->moccupation && !occupation){
				mtmp->moccupation = 0;
				mtmp->mcanmove = 1;
			}
		}
/* fainting stops your occupation, there's no reason to sync.
		sync_hunger();
*/
#ifdef REDO
		nomul(0, NULL);
		pushch(0);
#endif
	}
}

#endif /* OVL1 */
#ifdef OVLB

void
display_gamewindows()
{
    curses_stupid_hack = 0;
    WIN_MESSAGE = create_nhwindow(NHW_MESSAGE);
    WIN_STATUS = create_nhwindow(NHW_STATUS);
    WIN_MAP = create_nhwindow(NHW_MAP);
    WIN_INVEN = create_nhwindow(NHW_MENU);

#ifdef MAC
    /*
     * This _is_ the right place for this - maybe we will
     * have to split display_gamewindows into create_gamewindows
     * and show_gamewindows to get rid of this ifdef...
     */
	if ( ! strcmp ( windowprocs . name , "mac" ) ) {
	    SanePositions ( ) ;
	}
#endif

    /*
     * The mac port is not DEPENDENT on the order of these
     * displays, but it looks a lot better this way...
     */
    display_nhwindow(WIN_STATUS, FALSE);
    display_nhwindow(WIN_MESSAGE, FALSE);
    clear_glyph_buffer();
    display_nhwindow(WIN_MAP, FALSE);
}

void
newgame()
{
	int i;

#ifdef MFLOPPY
	gameDiskPrompt();
#endif

	flags.ident = 1;

	for (i = 0; i < NUMMONS; i++)
		mvitals[i].mvflags = mons[i].geno & G_NOCORPSE;

	init_objects();		/* must be before u_init() */
	id_permonst();		/* must be before u_init() */
	
	flags.pantheon = -1;	/* role_init() will reset this */
	flags.panLgod = -1;	/* role_init() will reset this */
	flags.panNgod = -1;	/* role_init() will reset this */
	flags.panCgod = -1;	/* role_init() will reset this */
	role_init(TRUE);		/* must be before init_dungeons(), u_init(),
				 * and init_artifacts() */
	
	init_dungeons();	/* must be before u_init() to avoid rndmonst()
				 * creating odd monsters for any tins and eggs
				 * in hero's initial inventory */
	init_artifacts();	/* before u_init() in case $WIZKIT specifies
				 * any artifacts */
	u_init();
	
	hack_artifacts();	/* recall after u_init() to fix up role specific artifacts */

#ifndef NO_SIGNAL
	(void) signal(SIGINT, (SIG_RET_TYPE) done1);
#endif
#ifdef NEWS
	if(iflags.news) display_file(NEWS, FALSE);
#endif
	load_qtlist();	/* load up the quest text info */
/*	quest_init();*/	/* Now part of role_init() */

	mklev();
	u_on_upstairs();
	vision_reset();		/* set up internals for level (after mklev) */
	check_special_room(FALSE);

	flags.botlx = 1;

	/* Move the monster from under you or else
	 * makedog() will fail when it calls makemon().
	 *			- ucsfcgl!kneller
	 */
	if(MON_AT(u.ux, u.uy)) mnexto(m_at(u.ux, u.uy));
	(void) makedog();
	if(Race_if(PM_ANDROID))
		scatter_weapons();
	docrt();
#ifdef CONVICT
       if (Role_if(PM_CONVICT)) {
              setworn(mkobj(CHAIN_CLASS, TRUE), W_CHAIN);
              setworn(mkobj(BALL_CLASS, TRUE), W_BALL);
              uball->spe = 1;
              placebc();
              newsym(u.ux,u.uy);
       }
#endif /* CONVICT */

	if (flags.legacy) {
		flush_screen(1);
        if(Role_if(PM_EXILE)){
			com_pager(217);
#ifdef CONVICT
		} else if (Role_if(PM_CONVICT)) {
		    com_pager(199);
#endif /* CONVICT */
        // } else if(Race_if(PM_ELF)){
			// com_pager(211);
		} else if(Race_if(PM_ELF) && (Role_if(PM_PRIEST) || Role_if(PM_RANGER) || Role_if(PM_NOBLEMAN) || Role_if(PM_WIZARD))){
			com_pager(211);
		} else if(Role_if(PM_ANACHRONONAUT)){
			com_pager(218);
			if(Race_if(PM_ANDROID)){
				com_pager(222);
				com_pager(223);
				com_pager(224);
				com_pager(225);
			} else {
				com_pager(219);
				com_pager(220);
				com_pager(221);
			}
		} else if(Race_if(PM_WORM_THAT_WALKS)){
			if(Role_if(PM_CONVICT)){
				com_pager(214);
			} else if(Race_if(PM_ELF) && (Role_if(PM_PRIEST) || Role_if(PM_RANGER) || Role_if(PM_NOBLEMAN) || Role_if(PM_WIZARD))){
				com_pager(213);
			} else{
				com_pager(212);
			}
			com_pager(215);
			com_pager(216);
		} else {
			com_pager(1);
		}
	}

#ifdef INSURANCE
	save_currentstate();
#endif
	program_state.something_worth_saving++;	/* useful data now exists */

#if defined(RECORD_REALTIME) || defined(REALTIME_ON_BOTL)

	/* Start the timer here */
	realtime_data.realtime = (time_t)0L;

#if defined(BSD) && !defined(POSIX_TYPES)
	(void) time((long *)&realtime_data.restoretime);
#else
	(void) time(&realtime_data.restoretime);
#endif

#endif /* RECORD_REALTIME || REALTIME_ON_BOTL */

	if(Darksight) litroom(FALSE,NULL);
	/* Success! */
	welcome(TRUE);
	if(Race_if(PM_INHERITOR)){
		int inherited;
		struct obj *otmp;
		do{inherited = do_inheritor_menu();}while(!inherited);
		otmp = mksobj((int)artilist[inherited].otyp, FALSE, FALSE);
	    otmp = oname(otmp, artilist[inherited].name);
		expert_weapon_skill(weapon_type(otmp));
		discover_artifact(inherited);
		fully_identify_obj(otmp);
	    otmp = hold_another_object(otmp, "Oops!  %s to the floor!",
				       The(aobjnam(otmp, "slip")), (const char *)0);
		if(otmp->oclass == WEAPON_CLASS)
			expert_weapon_skill(objects[otmp->otyp].oc_skill);
	    // otmp->oartifact = inherited;
	}
	return;
}

STATIC_OVL int
do_inheritor_menu()
{
	winid tmpwin;
	int n, how, i;
	char buf[BUFSZ];
	char incntlet = 'a';
	menu_item *selected;
	anything any;

	tmpwin = create_nhwindow(NHW_MENU);
	start_menu(tmpwin);
	any.a_void = 0;		/* zero out all bits */

	for (i = 1; i<=NROFARTIFACTS; i++)
	{
		// if ((artilist[i].spfx2) && artilist[i].spfx && artilist[i].spfx)
		if(artilist[i].gflags&ARTG_INHER
		&& !Role_if(artilist[i].role)
		&& !Pantheon_if(artilist[i].role)
		&& (artilist[i].alignment == A_NONE
			|| artilist[i].alignment == u.ualign.type)
		){
			Sprintf(buf, "%s", artilist[i].name);
			any.a_int = i;	/* must be non-zero */
			add_menu(tmpwin, NO_GLYPH, &any,
				incntlet, 0, ATR_NONE, buf,
				MENU_UNSELECTED);
			incntlet = (incntlet == 'z') ? 'A' : (incntlet == 'Z') ? 'a' : (incntlet + 1);
		}
	}

	end_menu(tmpwin, "Which artifact did you inherit?");

	how = PICK_ONE;
	n = select_menu(tmpwin, how, &selected);
	destroy_nhwindow(tmpwin);
	return (n > 0) ? selected[0].item.a_int : 0;
}

/* show "welcome [back] to nethack" message at program startup */
void
welcome(new_game)
boolean new_game;	/* false => restoring an old game */
{
    char buf[BUFSZ];
    boolean currentgend = Upolyd ? u.mfemale : flags.female;

    /*
     * The "welcome back" message always describes your innate form
     * even when polymorphed or wearing a helm of opposite alignment.
     * Alignment is shown unconditionally for new games; for restores
     * it's only shown if it has changed from its original value.
     * Sex is shown for new games except when it is redundant; for
     * restores it's only shown if different from its original value.
     */
    *buf = '\0';
    if (new_game || u.ualignbase[A_ORIGINAL] != u.ualignbase[A_CURRENT])
	Sprintf(eos(buf), " %s", align_str(u.ualignbase[A_ORIGINAL]));
    if (!urole.name.f &&
	    (new_game ? (urole.allow & ROLE_GENDMASK) == (ROLE_MALE|ROLE_FEMALE) :
	     currentgend != flags.initgend))
	Sprintf(eos(buf), " %s", genders[currentgend].adj);

    pline(new_game ? "%s %s, welcome to dNetHack!  You are a%s %s %s."
		   : "%s %s, the%s %s %s, welcome back to dNetHack!",
	  Hello((struct monst *) 0), plname, buf, urace.adj,
	  (currentgend && urole.name.f) ? urole.name.f : urole.name.m);
	if(iflags.dnethack_start_text){
	pline("Press Ctrl^W or type #ward to engrave a warding sign.");
	if(Role_if(PM_PIRATE)) You("can swim! Type #swim while swimming on the surface to dive down to the bottom.");
	else if(Role_if(PM_EXILE)){
		pline("Press Ctrl^E or type #seal to engrave a seal of binding.");
		pline("#chat to a fresh seal to contact the spirit beyond.");
		pline("Press Ctrl^F or type #power to fire active spirit powers!");
	}
	if(Race_if(PM_DROW)){
		pline("Beware, droven armor evaporates in light!");
		pline("Use #monster to create a patch of darkness.");
	}
	if(Race_if(PM_ANDROID)){
		pline("Androids do not need to eat, but *do* need to sleep.");
		pline("Use #monster to access your innate abilities, including sleep.");
		pline("Use '.' to recover HP using magic energy.");
		pline("Sleep to recover magic energy.");
	}
	if(Race_if(PM_CLOCKWORK_AUTOMATON)){
		pline("Use #monster to adjust your clockspeed.");
		You("do not heal naturally. Use '.' to attempt repairs.");
	}
	if(Race_if(PM_INCANTIFIER)){
		pline("Incantifiers eat magic, not food, and do not heal naturally.");
	}
	}
}

#ifdef POSITIONBAR
STATIC_DCL void
do_positionbar()
{
	static char pbar[COLNO];
	char *p;
	
	p = pbar;
	/* up stairway */
	if (upstair.sx &&
	   (glyph_to_cmap(level.locations[upstair.sx][upstair.sy].glyph) ==
	    S_upstair ||
 	    glyph_to_cmap(level.locations[upstair.sx][upstair.sy].glyph) ==
	    S_upladder)) {
		*p++ = '<';
		*p++ = upstair.sx;
	}
	if (sstairs.sx &&
	   (glyph_to_cmap(level.locations[sstairs.sx][sstairs.sy].glyph) ==
	    S_upstair ||
 	    glyph_to_cmap(level.locations[sstairs.sx][sstairs.sy].glyph) ==
	    S_upladder)) {
		*p++ = '<';
		*p++ = sstairs.sx;
	}

	/* down stairway */
	if (dnstair.sx &&
	   (glyph_to_cmap(level.locations[dnstair.sx][dnstair.sy].glyph) ==
	    S_dnstair ||
 	    glyph_to_cmap(level.locations[dnstair.sx][dnstair.sy].glyph) ==
	    S_dnladder)) {
		*p++ = '>';
		*p++ = dnstair.sx;
	}
	if (sstairs.sx &&
	   (glyph_to_cmap(level.locations[sstairs.sx][sstairs.sy].glyph) ==
	    S_dnstair ||
 	    glyph_to_cmap(level.locations[sstairs.sx][sstairs.sy].glyph) ==
	    S_dnladder)) {
		*p++ = '>';
		*p++ = sstairs.sx;
	}

	/* hero location */
	if (u.ux) {
		*p++ = '@';
		*p++ = u.ux;
	}
	/* fence post */
	*p = 0;

	update_positionbar(pbar);
}
#endif

#if defined(REALTIME_ON_BOTL) || defined (RECORD_REALTIME)
time_t
get_realtime(void)
{
    time_t curtime;

    /* Get current time */
#if defined(BSD) && !defined(POSIX_TYPES)
    (void) time((long *)&curtime);
#else
    (void) time(&curtime);
#endif

    /* Since the timer isn't set until the game starts, this prevents us
     * from displaying nonsense on the bottom line before it does. */
    if(realtime_data.restoretime == 0) {
        curtime = realtime_data.realtime;
    } else {
        curtime -= realtime_data.restoretime;
        curtime += realtime_data.realtime;
    }
 
    return curtime;
}
#endif /* REALTIME_ON_BOTL || RECORD_REALTIME */

STATIC_OVL void
cthulhu_mind_blast()
{
	struct monst *mon, *nmon;
	int nd = 1;
	if(on_level(&rlyeh_level,&u.uz))
		nd = 5;
	if (Unblind_telepat || (Blind_telepat && rn2(2)) || !rn2(10)) {
		int dmg;
		pline("It locks on to your %s!",
			Unblind_telepat ? "telepathy" :
			Blind_telepat ? "latent telepathy" : "mind");
		dmg = d(nd,15);
		if(Half_spell_damage) dmg = (dmg+1) / 2;
		if(u.uvaul_duration) dmg = (dmg + 1) / 2;
		losehp(dmg, "psychic blast", KILLED_BY_AN);
		make_stunned(HStun + dmg*10, TRUE);
		if (Sleep_resistance){
			if(!on_level(&rlyeh_level,&u.uz)) fall_asleep(-1*dmg, TRUE);
			if(!rn2(10)) change_usanity(-1);
		} else {
			if(!on_level(&rlyeh_level,&u.uz)) fall_asleep(-100*dmg, TRUE);
			change_usanity(-1);
		}
	}
	for(mon=fmon; mon; mon = nmon) {
		nmon = mon->nmon;
		if (DEADMONSTER(mon)) continue;
		if (is_mind_flayer(mon->data)) continue;
		if (mindless_mon(mon)) continue;
		if (mon_resistance(mon,TELEPAT) || !rn2(5)){
			mon->mhp -= d(nd,15);
			if (mon->mhp <= 0) mondied(mon);
			else {
				mon->mconf = 1;
				mon->msleeping = 1;
				slept_monst(mon);
			}
		}
		else mon->msleeping = 0;
	}
}

STATIC_OVL void
see_nearby_monsters()
{
	if(!Blind){
		int dx, dy;
		struct monst *mtmp;
		
		for(dx=-1; dx<2; dx++){
			for(dy=-1; dy<2; dy++){
				if(isok(u.ux+dx, u.uy+dy)){
					if((mtmp = m_at(u.ux+dx, u.uy+dy)) && !mtmp->mtame && canseemon(mtmp) && !(mvitals[monsndx(mtmp->data)].seen)){
						mvitals[monsndx(mtmp->data)].seen = 1;
						if(Role_if(PM_TOURIST)){
							more_experienced(experience(mtmp,0),0);
							newexplevel();
						}
						give_madness(mtmp);
						if(u.usanity > 0 && taxes_sanity(mtmp->data)){
							change_usanity(u_sanity_loss(mtmp));
						}
						if(yields_insight(mtmp->data)){
							change_uinsight(u_visible_insight(mtmp));
						}
						}
					}
				}
			}
	}
}

STATIC_OVL
void
unseen_actions(mon)
struct monst *mon;
{
	//Note: May cause mon to change its state, including moving to a different monster chain.
	if(mon->mux == u.uz.dnum && mon->muy == u.uz.dlevel && mon->mtyp == PM_BLESSED)
		blessed_spawn(mon);
	else if(mon->mux == u.uz.dnum && mon->muy == u.uz.dlevel && mon->mtyp == PM_THE_GOOD_NEIGHBOR)
		good_neighbor(mon);
	else if(mon->mux == u.uz.dnum && mon->muy == u.uz.dlevel && mon->mtyp == PM_HMNYW_PHARAOH)
		dark_pharaoh(mon);
	else if(mon->mux == u.uz.dnum && mon->muy == u.uz.dlevel && mon->mtyp == PM_POLYPOID_BEING)
		polyp_pickup(mon);
	else if(mon->mux == u.uz.dnum && mon->muy == u.uz.dlevel && mon->mtyp == PM_MOUTH_OF_THE_GOAT)
		goat_sacrifice(mon);
}

static int goatkids[] = {PM_SMALL_GOAT_SPAWN, PM_GOAT_SPAWN, PM_GIANT_GOAT_SPAWN, 
						 PM_SWIRLING_MIST, PM_DUST_STORM, PM_ICE_STORM, PM_THUNDER_STORM, PM_FIRE_STORM, 
						 PM_PLAINS_CENTAUR, PM_FOREST_CENTAUR, PM_MOUNTAIN_CENTAUR,
						 PM_QUICKLING, PM_DRYAD, PM_NAIAD, PM_OREAD, PM_YUKI_ONNA, PM_DEMINYMPH};
STATIC_OVL
void
blessed_spawn(mon)
struct monst *mon;
{
	struct monst *mtmp;
	xchar xlocale, ylocale, xyloc;
	xyloc	= mon->mtrack[0].x;
	xlocale = mon->mtrack[1].x;
	ylocale = mon->mtrack[1].y;
	if(xyloc == MIGR_EXACT_XY && !mon->mpeaceful && !rn2(70)){
		mtmp = makemon(&mons[goatkids[rn2(SIZE(goatkids))]], xlocale, ylocale, MM_ADJACENTOK|NO_MINVENT|MM_NOCOUNTBIRTH);
		if(mtmp){
			mtmp->mpeaceful = 0;
			set_malign(mtmp);
		}
	}
}

static int witchspawns[] = {PM_APPRENTICE_WITCH, PM_APPRENTICE_WITCH, PM_APPRENTICE_WITCH, PM_WITCH, 
						 PM_APPRENTICE_WITCH, PM_APPRENTICE_WITCH, PM_APPRENTICE_WITCH, PM_WITCH, 
						 PM_BABY_LONG_WORM, PM_BABY_PURPLE_WORM, PM_LONG_WORM, PM_PURPLE_WORM, PM_HUNTING_HORROR, 
						 PM_LARGE_CAT, PM_PANTHER, PM_TIGER, PM_GIANT_RAT, PM_ENORMOUS_RAT, PM_RODENT_OF_UNUSUAL_SIZE};
STATIC_OVL
void
good_neighbor(mon)
struct monst *mon;
{
	struct monst *mtmp;
	xchar xlocale, ylocale, xyloc;
	xyloc	= mon->mtrack[0].x;
	xlocale = mon->mtrack[1].x;
	ylocale = mon->mtrack[1].y;
	if(xyloc == MIGR_EXACT_XY){
		if(u.ux == xlocale && u.uy == ylocale && !mon->mpeaceful){
			You_feel("someone walking on your grave!");
			change_luck(-13);
		} else if(!mon->mpeaceful && !rn2(140)){
			mtmp = makemon(&mons[witchspawns[rn2(SIZE(witchspawns))]], xlocale, ylocale, MM_ADJACENTOK|MM_NOCOUNTBIRTH);
			if(mtmp){
				mtmp->mpeaceful = 0;
				set_malign(mtmp);
			}
		} else if((mtmp = m_at(xlocale, ylocale))){
			if(mtmp->mtyp == PM_APPRENTICE_WITCH
				|| mtmp->mtyp == PM_WITCH
				|| mtmp->mtyp == PM_COVEN_LEADER
				|| mtmp->mtyp == PM_WITCH_S_FAMILIAR
			){
				//Heal and fix troubles
				if(needs_familiar(mtmp)){
					struct monst *familliar;
					familliar = makemon(&mons[PM_WITCH_S_FAMILIAR], mtmp->mx, mtmp->my, MM_ADJACENTOK|MM_NOCOUNTBIRTH);
					if(familliar){
						//Sync new familiar
						familliar->m_lev = mtmp->m_lev;
						familliar->mhpmax = mtmp->mhpmax;
						familliar->mhp = familliar->mhpmax;
						familliar->mvar_witchID = (long)mtmp->m_id;
						familliar->mpeaceful = mtmp->mpeaceful;
						//Stop running
						if(mtmp->mflee && mtmp->mhp > mtmp->mhpmax/2){
							mtmp->mflee = 0;
							mtmp->mfleetim = 0;
						}
					}
				}
				mtmp->mhp = mtmp->mhpmax;
				mtmp->mspec_used = 0;
				mtmp->mstdy = 0;
				mtmp->ustdym = 0;
				mtmp->mcan = 0;
				
				mtmp->mflee = 0;
				mtmp->mfleetim = 0;
				mtmp->mcansee = 1;
				mtmp->mblinded = 0;
				mtmp->mcanhear = 1;
				mtmp->mdeafened = 0;
				mtmp->mcanmove = 1;
				mtmp->mfrozen = 0;
				mtmp->msleeping = 0;
				mtmp->mstun = 0;
				mtmp->mconf = 0;
				mtmp->mtrapped = 0;
				mtmp->entangled = 0;
			}
		}
		xlocale += rn2(3) - 1;
		ylocale += rn2(3) - 1;
		if(isok(xlocale, ylocale) && !is_pool(xlocale, ylocale, FALSE) && ZAP_POS(levl[xlocale][ylocale].typ)){
			mon->mtrack[1].x = xlocale;
			mon->mtrack[1].y = ylocale;
		}
	}
}

static int pharaohspawns[] = {PM_COBRA, PM_COBRA, PM_COBRA, PM_SERPENT_NECKED_LIONESS, PM_HUNTING_HORROR,
							  PM_COBRA, PM_COBRA, PM_COBRA, PM_SERPENT_NECKED_LIONESS, PM_HUNTING_HORROR,
							  PM_HUMAN_MUMMY, PM_HUMAN_MUMMY, PM_HUMAN_MUMMY, PM_GIANT_MUMMY, PM_PHARAOH,
							  PM_ENERGY_VORTEX, PM_ENERGY_VORTEX, PM_ENERGY_VORTEX, PM_LIGHTNING_PARAELEMENTAL, PM_BLUE_DRAGON};

STATIC_OVL
void
dark_pharaoh(mon)
struct monst *mon;
{
	struct monst *mtmp;
	xchar xlocale, ylocale, xyloc;
	xyloc	= mon->mtrack[0].x;
	xlocale = mon->mtrack[1].x;
	ylocale = mon->mtrack[1].y;
	struct obj *otmp;
	for (mtmp = fmon; mtmp; mtmp = mtmp->nmon){
		if(mtmp->mtyp != PM_NITOCRIS && mtmp->mtyp != PM_GHOUL_QUEEN_NITOCRIS)
			continue;
		//found her
		if(xyloc == MIGR_EXACT_XY){
			mon->mtrack[1].x = mtmp->mx;
			mon->mtrack[1].y = mtmp->my;
			xlocale = mon->mtrack[1].x;
			ylocale = mon->mtrack[1].y;
			if(which_armor(mtmp, W_ARMC) && which_armor(mtmp, W_ARMC)->oartifact == ART_PRAYER_WARDED_WRAPPINGS_OF){
				//Directed to cast spells on her behalf
				mtmp->mspec_used = 0;
			} else if(mtmp->mtyp == PM_NITOCRIS){
				//No longer protected, kill her
				if(canseemon(mtmp)){
					pline("Dark waters pour into %s's mouth and throat!", mon_nam(mtmp));
					pline("%s drowns!", Monnam(mtmp));
				}
				mtmp->mhp = 0;
				mondied(mtmp);
			} else if(mtmp->mtyp == PM_GHOUL_QUEEN_NITOCRIS){
				if(mtmp->mhp < mtmp->mhpmax/2){
					mtmp->mhp = mtmp->mhpmax;
					if(canseemon(mtmp)) pline("Dark waters seal %s's wounds!", mon_nam(mtmp));
				} else {
					mtmp->mhp = min(mtmp->mhp+9, mtmp->mhpmax);
					mtmp->mspec_used = 0;
					mtmp->mstdy = 0;
					mtmp->ustdym = 0;
					mtmp->mcan = 0;
					
					mtmp->mflee = 0;
					mtmp->mfleetim = 0;
					mtmp->mcansee = 1;
					mtmp->mblinded = 0;
					mtmp->mcanhear = 1;
					mtmp->mdeafened = 0;
					mtmp->mcanmove = 1;
					mtmp->mfrozen = 0;
					mtmp->msleeping = 0;
					mtmp->mstun = 0;
					mtmp->mconf = 0;
					mtmp->mtrapped = 0;
					mtmp->entangled = 0;
				}
			}
		}
		if(!rn2(140)){
			struct permonst *pm;
			pm = &mons[pharaohspawns[rn2(SIZE(pharaohspawns))]];
			mtmp = makemon(pm, xlocale, ylocale, MM_ADJACENTOK|MM_NOCOUNTBIRTH);
			if(mtmp){
				mtmp->mpeaceful = 0;
				set_malign(mtmp);
			}
			mtmp = makemon(pm, xlocale, ylocale, MM_ADJACENTOK|MM_NOCOUNTBIRTH);
			if(mtmp){
				mtmp->mpeaceful = 0;
				set_malign(mtmp);
			}
		}
		return;//No further action.
	}
	struct obj *obj = 0;
	for (obj = fobj; obj; obj = obj->nobj) {
		if(obj->otyp == CORPSE && (obj->corpsenm == PM_NITOCRIS || obj->corpsenm == PM_GHOUL_QUEEN_NITOCRIS))
			break;
	}
	if(!obj) for (obj = invent; obj; obj = obj->nobj) {
		if(obj->otyp == CORPSE && (obj->corpsenm == PM_NITOCRIS || obj->corpsenm == PM_GHOUL_QUEEN_NITOCRIS))
			break;
	}
	if(!obj) for (mtmp = fmon; mtmp; mtmp = mtmp->nmon) {
		if(obj) break;
		if (DEADMONSTER(mtmp)) continue;
		for (obj = mtmp->minvent; obj; obj = obj->nobj) {
			if(obj->otyp == CORPSE && (obj->corpsenm == PM_NITOCRIS || obj->corpsenm == PM_GHOUL_QUEEN_NITOCRIS))
				break;
		}
	}
	if(obj){
		if(get_obj_location(obj, &xlocale, &ylocale, 0)){
			mon->mtrack[1].x = xlocale;
			mon->mtrack[1].y = ylocale;
		}
		if(cansee(xlocale, ylocale)) pline("Dark waters swallow Nitocris!");
		mtmp = revive(obj, FALSE);
		if(mtmp)
			rloc(mtmp, FALSE);
		return;//No further action.
	}
	if(!rn2(70)){
		struct permonst *pm;
		pm = &mons[pharaohspawns[rn2(SIZE(pharaohspawns))]];
		mtmp = makemon(pm, xlocale, ylocale, MM_ADJACENTOK|MM_NOCOUNTBIRTH);
		if(mtmp){
			mtmp->mpeaceful = 0;
			set_malign(mtmp);
		}
	}
}

STATIC_OVL
void
polyp_pickup(mon)
struct monst *mon;
{
	struct obj *otmp, *otmp2;
	register struct monst *mtmp, *mtmp0 = 0, *mtmp2;
	xchar xlocale, ylocale, xyloc;
	xyloc	= mon->mtrack[0].x;
	xlocale = mon->mtrack[1].x;
	ylocale = mon->mtrack[1].y;
	if(xyloc == MIGR_EXACT_XY){
		if(m_at(xlocale, ylocale))
			return;
		if(xlocale == u.ux && ylocale == u.uy)
			return;
		if(!ZAP_POS(levl[xlocale][ylocale].typ))
			return;
		for(otmp = level.objects[xlocale][ylocale]; otmp; otmp = otmp2){
			otmp2 = otmp->nexthere;
			if(otmp->otyp == MASK && !otmp->oartifact && !(mons[otmp->corpsenm].geno&G_UNIQ)){
				obj_extract_self(otmp);
				/* unblock point after extract, before pickup */
				if (is_boulder(otmp)) /*Shouldn't be a boulder, but who knows if a huge mask will get invented*/
					unblock_point(xlocale,ylocale);	/* vision */
				if(otmp) (void) mpickobj(mon, otmp);	/* may merge and free otmp */
				newsym(xlocale,ylocale);
			}
		}
		for(otmp = mon->minvent; otmp; otmp = otmp->nobj){
			if(otmp->otyp == MASK && !otmp->oartifact && !(mons[otmp->corpsenm].geno&G_UNIQ)){
				for(mtmp = migrating_mons; mtmp; mtmp = mtmp2) {
					mtmp2 = mtmp->nmon;
					if (mtmp == mon) {
						if(mtmp == migrating_mons)
							migrating_mons = mtmp->nmon;
						else
							mtmp0->nmon = mtmp->nmon;
						mon_arrive(mtmp, FALSE);
						break;
					} else
						mtmp0 = mtmp;
				}
				if(mtmp){
					/*mtmp and mon *should* now be the same.  However, only do the polymorph if we have successfully removed the monster from the migrating chain and placed it!*/
					int pm = otmp->corpsenm;
					if(canseemon(mon))
						pline("%s puts on a mask!", Monnam(mon));
					m_useup(mon, otmp);
					mon->ispolyp = TRUE;
					newcham(mon, pm, FALSE, FALSE);
					mon->m_insight_level = 0;
					m_dowear(mon, TRUE);
					init_mon_wield_item(mon);

					/*Break out of loop. Warning Note: otmp is stale*/
					break;
				}
			}
		}
	}
}

STATIC_OVL
void
goat_sacrifice(mon)
struct monst *mon;
{
	struct obj *otmp, *otmp2;
	register struct monst *mtmp, *mtmp0 = 0, *mtmp2;
	xchar xlocale, ylocale, xyloc;
	xyloc	= mon->mtrack[0].x;
	xlocale = mon->mtrack[1].x;
	ylocale = mon->mtrack[1].y;
	if(xyloc == MIGR_EXACT_XY){
		for(otmp = level.objects[xlocale][ylocale]; otmp; otmp = otmp2){
			otmp2 = otmp->nexthere;
			if(otmp->otyp == CORPSE && !otmp->oartifact){
				goat_eat(otmp); //No matter what, the this function should remove this corpse.  Either via resurrection or destruction
				//Warning note: otmp is now stale
				return;
			}
		}
		if(!isok(xlocale, ylocale))
			return;
		//else
		if(xlocale == u.ux && ylocale == u.uy){
			if(!rn2(20)) switch(rnd(3)){
				case 1:
					pline("The shadowy mist forms briefly into a yawning maw!");
				break;
				case 2:
					pline("The dark forms into hooves and writhing tendrils!");
				break;
				case 3:
					if(uarmh && uarmh->otyp == SEDGE_HAT){
						pline("A few drops of liquid hit your wide straw hat.");
					} else if(uarmh && uarmh->otyp == WAR_HAT) {
						pline("A few drops of liquid hit your wide helm.");
					} else if(uarmh && uarmh->otyp == WITCH_HAT) {
						pline("A few drops of liquid hit your wide conical hat.");
					} else if(magic_negation(&youmonst)){
						pline("A few drops of liquid drip onto your clothes.");
					} else {
						int dmg = d(1, 3);
						pline("A few drops of spittle drip onto you.");
						if (!Acid_resistance) {
							pline("It burns!");
							losehp(dmg, "hungry goat", KILLED_BY_AN);
						}
					}
				break;
			}
		} else if(m_at(xlocale, ylocale)){
			struct monst *mtmp = m_at(xlocale, ylocale);
			if(!rn2(60)){
				if(which_armor(mtmp, W_ARMH) && which_armor(mtmp, W_ARMH)->otyp == SEDGE_HAT){
					if(canseemon(mtmp))
						pline("A few drops of viscous liquid hit %s wide straw hat.", s_suffix(mon_nam(mtmp)));
				} else if(which_armor(mtmp, W_ARMH) && which_armor(mtmp, W_ARMH)->otyp == WAR_HAT) {
					if(canseemon(mtmp))
						pline("A few drops of viscous liquid hit %s wide helm.", s_suffix(mon_nam(mtmp)));
				} else if(which_armor(mtmp, W_ARMH) && which_armor(mtmp, W_ARMH)->otyp == WITCH_HAT) {
					if(canseemon(mtmp))
						pline("A few drops of viscous liquid hit %s wide conical hat.", s_suffix(mon_nam(mtmp)));
				} else if(magic_negation(mtmp)){
					if(canseemon(mtmp))
						pline("A few drops of viscous liquid hit %s clothes.", s_suffix(mon_nam(mtmp)));
				} else {
					int dmg = d(1, 3);
					if(canseemon(mtmp))
						pline("A few drops of viscous liquid hit %s.", mon_nam(mtmp));
					if (!resists_acid(mtmp)){
						if(mtmp->data->mmove) //Not naturally inactive
							pline("%s winces!",Monnam(mtmp));
						mtmp->mhp -= dmg;
						if(mtmp->mhp <= 0)
							mondied(mtmp);
					}
				}
			}
		} else {
			if(!rn2(60)){
				if(cansee(xlocale, ylocale)){
					You("see a viscous liquid dripping onto %s.", the(surface(xlocale, ylocale)));
					map_invisible(xlocale, ylocale);
				}
				else {
					You_hear("a viscous liquid dripping onto %s.", the(surface(xlocale, ylocale)));
				}
			}
		}
	}
}

void
dogoat()
{
	struct monst *mon;
	int tmp, weptmp, tchtmp;
	int clockwisex[8] = { 0, 1, 1, 1, 0,-1,-1,-1};
	int clockwisey[8] = {-1,-1, 0, 1, 1, 1, 0,-1};
	int i = rnd(8),j, lim=0;
	struct attack symbiote = { AT_TENT, AD_PHYS, 4, 4 };
	for(j=8;j>=1;j--){
		if(u.ustuck && u.uswallow)
			mon = u.ustuck;
		else if(!isok(u.ux+clockwisex[(i+j)%8], u.uy+clockwisey[(i+j)%8]))
			continue;
		else mon = m_at(u.ux+clockwisex[(i+j)%8], u.uy+clockwisey[(i+j)%8]);
		if(!mon || mon->mpeaceful)
			continue;
		if(touch_petrifies(mon->data)
		 || mon->mtyp == PM_MEDUSA
		 || mon->mtyp == PM_PALE_NIGHT
		) continue;
		
		if(mon && !mon->mtame){
			xmeleehity(&youmonst, mon, &symbiote, (struct obj *)0, -1, 0, FALSE);
		}
	}
}

#endif /* OVLB */

/*allmain.c*/
