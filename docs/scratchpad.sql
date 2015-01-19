
use [gamescheduler];
go

select * from DataContext

select * from Game

select * from Player


--select dc.Id as [DataContextId], dc.Name as [DataContextName],
--g.Id as [GameId], g.Name as [GameName],
--p.Id as [PlayerId], p.Name as [PlayerName],
--(g.H*p.H) as [H], (g.E*p.E) as [E], (g.P*p.P) as [P],
--((g.H*p.H)+(g.E*p.E)+(g.P*p.P)) as [Value]
--from Game g inner join Preference p on g.Id=p.GameId
--inner join Player p on p.Id=p.PlayerId
--inner join DataContext dc on g.DataContextId=dc.Id and p.DataContextId=dc.Id
--order by [Value] desc

select * from RawPotentialValueView

select MAX(DataContextName) as [DataContextName],
MAX(GameName) as [GameName],
SUM(H) as [H], SUM(E) as [E], SUM(P) as [P],
SUM(Value) as [Value]
from AssignedValueView v
where DataContextName like '%bus%'
group by v.GameId

--just staying focused on Player preference comes up short: ~1993/2000 games filled, and some partially
--expand those horizons a little bit beyond Player preference and coverage is 100%
select max(g.name) as gamename,
count(p.id) as size
from assigned a
inner join Game g on g.id=a.gameid
inner join datacontext dc on dc.id=g.datacontextid and dc.name like '%bus%'
left join player p on p.datacontextid=dc.id and p.id=a.playerid
group by g.id


select * from EmailSummaryView
where GameName = 'C1970'


