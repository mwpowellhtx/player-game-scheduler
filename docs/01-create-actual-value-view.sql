
create view AssignedValueView as
select dc.Id as [DataContextId], dc.Name as [DataContextName],
g.Id as [GameId], g.Name as [GameName],
p.Id as [PlayerId], p.Name as [PlayerName],
(g.H*p.H) as [H], (g.E*p.E) as [E], (g.P*p.P) as [P],
((g.H*p.H)+(g.E*p.E)+(g.P*p.P)) as [Value]
from Game g inner join Assigned a on g.Id=a.GameId
inner join Player p on p.Id=a.PlayerId
inner join DataContext dc on g.DataContextId=dc.Id and p.DataContextId=dc.Id
go
