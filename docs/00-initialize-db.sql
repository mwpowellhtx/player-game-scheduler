use [gamescheduler];
go

create table [DataContext] (
[Id] bigint not null identity(1, 1) constraint [PK_DataContext] primary key
, [Name] nvarchar(127) not null constraint [DF_DataContext_Name] default ''
)
go

--could potentially merge Game+Player into the same table
--but this is more effort for sake of example
create table [Game] (
[Id] bigint not null identity(1, 1) constraint [PK_Game] primary key
, [DataContextId] bigint null
  constraint [FK_Game_DataContext] foreign key
  references [DataContext] ([Id])
  on update cascade on delete cascade
, [Name] nvarchar(24) not null constraint [DF_Game_Name] default ''
, [NameId] int not null constraint [DF_Game_NameId] default 0
, [H] int not null constraint [DF_Game_H] default 0
, [E] int not null constraint [DF_Game_E] default 0
, [P] int not null constraint [DF_Game_P] default 0
)
go

create table [Player] (
[Id] bigint not null identity(1, 1) constraint [PK_Player] primary key
, [DataContextId] bigint null
  constraint [FK_Player_DataContext] foreign key
  references [DataContext] ([Id])
  on update cascade on delete cascade
, [Name] nvarchar(24) not null constraint [DF_Player_Name] default ''
, [NameId] int not null constraint [DF_Player_NameId] default 0
, [H] int not null constraint [DF_Player_H] default 0
, [E] int not null constraint [DF_Player_E] default 0
, [P] int not null constraint [DF_Player_P] default 0
)
go

--same goes for 'alignment' as compared with 'scheduled'
--more effort for sake of example
create table [Preference] (
[Id] bigint not null identity(1, 1) constraint [PK_Preference] primary key
, [GameId] bigint null
  constraint [DF_Preference_Game] default null
  constraint [FK_Preference_Game] foreign key
  references [Game] ([Id])
  on update cascade on delete cascade
, [PlayerId] bigint null
  constraint [DF_Preference_Player] default null
  constraint [FK_Preference_Player] foreign key
  references [Player] ([Id])
)
go

create table [Assigned] (
[Id] bigint not null identity(1, 1) constraint [PK_Assigned] primary key
, [GameId] bigint null
  constraint [DF_Assigned_Game] default null
  constraint [FK_Assigned_Game] foreign key
  references [Game] ([Id])
  on update cascade on delete cascade
, [PlayerId] bigint null
  constraint [DF_Assigned_Player] default null
  constraint [FK_Assigned_Player] foreign key
  references [Player] ([Id])
)
go

create trigger [TR_Player_After_Delete] on [Player]
after delete
as begin
set nocount on
delete from [Preference] where [PlayerId] in (select d.[Id] from [deleted] d)
delete from [Assigned] where [PlayerId] in (select d.[Id] from [deleted] d)
end
go

--shall not concern myself with update scenarios
-- once fk's are set, always set, for sake of example
