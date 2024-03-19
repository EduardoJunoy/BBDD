WITH boss
     AS (SELECT employeenumber
         FROM   employees
         WHERE  reportsto IS NULL),
     employee1
     AS (SELECT e.employeenumber
         FROM   employees e,
                boss b
         WHERE  e.reportsto = b.employeenumber)
SELECT e.employeenumber,
       e.lastname
FROM   employee1 e1,
       employees e
WHERE  e.reportsto = e1.employeenumber; 