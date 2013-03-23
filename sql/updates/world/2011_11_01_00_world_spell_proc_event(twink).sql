--Twink
DELETE FROM `spell_proc_event` WHERE `entry` IN ('11185','12487','12488');
INSERT INTO `spell_proc_event` (`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`) VALUES
(11185,	0x00,	3,	0x00000080,	0x00000000,	0x00000000,	0x00050000,	0x00000000,	0,	0,	0),
(12487,	0x00,	3,	0x00000080,	0x00000000,	0x00000000,	0x00050000,	0x00000000,	0,	0,	0),
(12488,	0x00,	3,	0x00000080,	0x00000000,	0x00000000,	0x00050000,	0x00000000,	0,	0,	0);