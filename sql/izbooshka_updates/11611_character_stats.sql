ALTER TABLE `character_stats`
ADD COLUMN `pos_strength` int(10) unsigned NOT NULL DEFAULT '0' AFTER `spirit`,
ADD COLUMN `pos_agility` int(10) unsigned NOT NULL DEFAULT '0' AFTER `pos_strength`,
ADD COLUMN `pos_stamina` int(10) unsigned NOT NULL DEFAULT '0' AFTER `pos_agility`,
ADD COLUMN `pos_intellect` int(10) unsigned NOT NULL DEFAULT '0' AFTER `pos_stamina`,
ADD COLUMN `pos_spirit` int(10) unsigned NOT NULL DEFAULT '0' AFTER `pos_intellect`,
ADD COLUMN `neg_strength` int(10) unsigned NOT NULL DEFAULT '0' AFTER `pos_spirit`,
ADD COLUMN `neg_agility` int(10) unsigned NOT NULL DEFAULT '0' AFTER `neg_strength`,
ADD COLUMN `neg_stamina` int(10) unsigned NOT NULL DEFAULT '0' AFTER `neg_agility`,
ADD COLUMN `neg_intellect` int(10) unsigned NOT NULL DEFAULT '0' AFTER `neg_stamina`,
ADD COLUMN `neg_spirit` int(10) unsigned NOT NULL DEFAULT '0' AFTER `neg_intellect`,
ADD COLUMN `baseatttime` int(10) unsigned NOT NULL DEFAULT '0' AFTER `spellPower`,
ADD COLUMN `mindamage` int(10) unsigned NOT NULL DEFAULT '0' AFTER `baseatttime`,
ADD COLUMN `maxdamage` int(10) unsigned NOT NULL DEFAULT '0' AFTER `mindamage`;