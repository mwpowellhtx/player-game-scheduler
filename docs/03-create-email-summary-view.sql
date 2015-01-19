use [gamescheduler];
go

create view EmailSummaryView as
select g.Name as [GameName],
SUM(p.NameId) as EmailId
from Game g
inner join Assigned a on a.GameId=g.Id
inner join Player p on p.Id=a.PlayerId
group by g.Name
go
