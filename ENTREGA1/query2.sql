SELECT productline,
       Avg(shippeddate - orderdate) AS avg_time
FROM   productlines
       natural JOIN products
       natural JOIN orderdetails
       natural JOIN orders
GROUP  BY productline; 