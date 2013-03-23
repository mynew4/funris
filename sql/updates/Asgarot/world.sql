--Фикс маникенов
update creature_template set unit_flags = 131072 where unit_flags = 393220 and Scriptname = 'npc_training_dummy';