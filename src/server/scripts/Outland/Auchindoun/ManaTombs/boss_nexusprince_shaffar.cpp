/*
 * This file is part of the TrinityCore Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/* ScriptData
SDName: Boss_NexusPrince_Shaffar
SD%Complete: 80
SDComment: Need more tuning of spell timers, it should not be as linear fight as current. Also should possibly find a better way to deal with his three initial beacons to make sure all aggro.
SDCategory: Auchindoun, Mana Tombs
EndScriptData */

#include "ScriptMgr.h"
#include "mana_tombs.h"
#include "MotionMaster.h"
#include "ScriptedCreature.h"

enum Yells
{
    SAY_INTRO                       = 0,
    SAY_AGGRO                       = 1,
    SAY_SLAY                        = 2,
    SAY_SUMMON                      = 3,
    SAY_DEAD                        = 4
};

enum Spells
{
    SPELL_BLINK                     = 34605,
    SPELL_FROSTBOLT                 = 32364,
    SPELL_FIREBALL                  = 32363,
    SPELL_FROSTNOVA                 = 32365,

    SPELL_ETHEREAL_BEACON           = 32371,                // Summons NPC_BEACON
    SPELL_ETHEREAL_BEACON_VISUAL    = 32368,

    // Ethereal Beacon
    SPELL_ARCANE_BOLT               = 15254,
    SPELL_ETHEREAL_APPRENTICE       = 32372                 // Summon 18430
};

enum Creatures
{
    NPC_BEACON                      = 18431,
    NPC_SHAFFAR                     = 18344
};

enum Misc
{
    NR_INITIAL_BEACONS              = 3
};

enum Events
{
    EVENT_BLINK                     = 1,
    EVENT_BEACON,
    EVENT_FIREBALL,
    EVENT_FROSTBOLT,
    EVENT_FROST_NOVA
};

struct boss_nexusprince_shaffar : public BossAI
{
    boss_nexusprince_shaffar(Creature* creature) : BossAI(creature, DATA_NEXUSPRINCE_SHAFFAR)
    {
        _hasTaunted = false;
    }

    void Reset() override
    {
        _Reset();

        float dist = 8.0f;
        float posX, posY, posZ, angle;
        me->GetHomePosition(posX, posY, posZ, angle);

        me->SummonCreature(NPC_BEACON, posX - dist, posY - dist, posZ, angle, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 2h);
        me->SummonCreature(NPC_BEACON, posX - dist, posY + dist, posZ, angle, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 2h);
        me->SummonCreature(NPC_BEACON, posX + dist, posY, posZ, angle, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 2h);
    }

    void MoveInLineOfSight(Unit* who) override
    {
        if (!_hasTaunted && who->GetTypeId() == TYPEID_PLAYER && me->IsWithinDistInMap(who, 100.0f))
        {
            Talk(SAY_INTRO);
            _hasTaunted = true;
        }
    }

    void JustEngagedWith(Unit* who) override
    {
        Talk(SAY_AGGRO);
        BossAI::JustEngagedWith(who);

        events.ScheduleEvent(EVENT_BEACON, 10s);
        events.ScheduleEvent(EVENT_FIREBALL, 8s);
        events.ScheduleEvent(EVENT_FROSTBOLT, 4s);
        events.ScheduleEvent(EVENT_FROST_NOVA, 15s);
    }

    void JustSummoned(Creature* summoned) override
    {
        if (summoned->GetEntry() == NPC_BEACON)
        {
            summoned->CastSpell(summoned, SPELL_ETHEREAL_BEACON_VISUAL, false);

            if (Unit* target = SelectTarget(SelectTargetMethod::Random, 0))
                summoned->AI()->AttackStart(target);
        }

        summons.Summon(summoned);
    }

    void KilledUnit(Unit* victim) override
    {
        if (victim->GetTypeId() == TYPEID_PLAYER)
            Talk(SAY_SLAY);
    }

    void JustDied(Unit* /*killer*/) override
    {
        Talk(SAY_DEAD);
        _JustDied();
    }

    void ExecuteEvent(uint32 eventId) override
    {
        switch (eventId)
        {
            case EVENT_BLINK:
                if (me->IsNonMeleeSpellCast(false))
                    me->InterruptNonMeleeSpells(true);

                // expire movement, will prevent from running right back to victim after cast
                // (but should MoveChase be used again at a certain time or should he not move?)
                me->GetMotionMaster()->Clear(MOTION_PRIORITY_NORMAL);

                DoCast(me, SPELL_BLINK);
                break;
            case EVENT_BEACON:
                if (!urand(0, 3))
                    Talk(SAY_SUMMON);

                DoCast(me, SPELL_ETHEREAL_BEACON, true);
                events.ScheduleEvent(EVENT_BEACON, 10s);
                break;
            case EVENT_FIREBALL:
                DoCastVictim(SPELL_FROSTBOLT);
                events.ScheduleEvent(EVENT_FIREBALL, 4500ms, 6s);
                break;
            case EVENT_FROSTBOLT:
                DoCastVictim(SPELL_FROSTBOLT);
                events.ScheduleEvent(EVENT_FROSTBOLT, 4500ms, 6s);
                break;
            case EVENT_FROST_NOVA:
                DoCast(me, SPELL_FROSTNOVA);
                events.ScheduleEvent(EVENT_FROST_NOVA, 17500ms, 25s);
                events.ScheduleEvent(EVENT_BLINK, 1500ms);
                break;
            default:
                break;
        }
    }

private:
    bool _hasTaunted;
};

enum EtherealBeacon
{
    EVENT_APPRENTICE = 1,
    EVENT_ARCANE_BOLT
};

struct npc_ethereal_beacon : public ScriptedAI
{
    npc_ethereal_beacon(Creature* creature) : ScriptedAI(creature) { }

    void Reset() override
    {
        _events.Reset();
    }

    void JustEngagedWith(Unit* who) override
    {
        if (Creature* shaffar = me->FindNearestCreature(NPC_SHAFFAR, 100.0f))
            if (!shaffar->IsInCombat())
                shaffar->AI()->AttackStart(who);

        _events.ScheduleEvent(EVENT_APPRENTICE, DUNGEON_MODE(20s, 10s));
        _events.ScheduleEvent(EVENT_ARCANE_BOLT, 1s);
    }

    void JustSummoned(Creature* summoned) override
    {
        summoned->AI()->AttackStart(me->GetVictim());
    }

    void UpdateAI(uint32 diff) override
    {
        if (!UpdateVictim())
            return;

        _events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_APPRENTICE:
                    DoCast(me, SPELL_ETHEREAL_APPRENTICE, true);
                    me->DespawnOrUnsummon();
                    break;
                case EVENT_ARCANE_BOLT:
                    DoCastVictim(SPELL_ARCANE_BOLT);
                    _events.ScheduleEvent(EVENT_ARCANE_BOLT, 2s, 4500ms);
                    break;
                default:
                    break;
            }
        }
    }

private:
    EventMap _events;
};

enum EtherealApprentice
{
    SPELL_ETHEREAL_APPRENTICE_FIREBOLT          = 32369,
    SPELL_ETHEREAL_APPRENTICE_FROSTBOLT         = 32370,
    EVENT_ETHEREAL_APPRENTICE_FIREBOLT          = 1,
    EVENT_ETHEREAL_APPRENTICE_FROSTBOLT
};

struct npc_ethereal_apprentice : public ScriptedAI
{
    npc_ethereal_apprentice(Creature* creature) : ScriptedAI(creature) { }

    void Reset() override
    {
        _events.Reset();
    }

    void JustEngagedWith(Unit* /*who*/) override
    {
        _events.ScheduleEvent(EVENT_ETHEREAL_APPRENTICE_FIREBOLT, 3s);
    }

    void UpdateAI(uint32 diff) override
    {
        if (!UpdateVictim())
            return;

        _events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_ETHEREAL_APPRENTICE_FIREBOLT:
                    DoCastVictim(SPELL_ETHEREAL_APPRENTICE_FIREBOLT, true);
                    _events.ScheduleEvent(EVENT_ETHEREAL_APPRENTICE_FROSTBOLT, 3s);
                    break;
                case EVENT_ETHEREAL_APPRENTICE_FROSTBOLT:
                    DoCastVictim(SPELL_ETHEREAL_APPRENTICE_FROSTBOLT, true);
                    _events.ScheduleEvent(EVENT_ETHEREAL_APPRENTICE_FIREBOLT, 3s);
                    break;
                default:
                    break;
            }
        }
    }

private:
    EventMap _events;
};

enum Yor
{
    SPELL_DOUBLE_BREATH          = 38361,
    EVENT_DOUBLE_BREATH          = 1
};

struct npc_yor : public ScriptedAI
{
    npc_yor(Creature* creature) : ScriptedAI(creature) { }

    void Reset() override { }

    void JustEngagedWith(Unit* /*who*/) override
    {
        _events.ScheduleEvent(EVENT_DOUBLE_BREATH, 6s, 9s);
    }

    void UpdateAI(uint32 diff) override
    {
        if (!UpdateVictim())
            return;

        _events.Update(diff);

        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_DOUBLE_BREATH:
                    if (me->IsWithinDist(me->GetVictim(), ATTACK_DISTANCE))
                        DoCastVictim(SPELL_DOUBLE_BREATH);
                    _events.ScheduleEvent(EVENT_DOUBLE_BREATH, 6s, 9s);
                    break;
                default:
                    break;
            }
        }
    }

    private:
        EventMap _events;
};

void AddSC_boss_nexusprince_shaffar()
{
    RegisterManaTombsCreatureAI(boss_nexusprince_shaffar);
    RegisterManaTombsCreatureAI(npc_ethereal_beacon);
    RegisterManaTombsCreatureAI(npc_ethereal_apprentice);
    RegisterManaTombsCreatureAI(npc_yor);
}
