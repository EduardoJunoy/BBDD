WITH yearorders
     AS (SELECT ordernumber
         FROM   orders
         WHERE  orders.orderdate >= '2003-01-01'
                AND orders.orderdate <= '2003-12-31'
        ),
     noorders
     AS (SELECT officecode,
                country
         FROM   offices OF
         WHERE  NOT EXISTS(SELECT ordernumber
                           FROM   yearorders
                                  natural JOIN customers
                                  JOIN employees
                                    ON employeenumber = salesrepemployeenumber
                           WHERE  officecode = OF.officecode))
SELECT country,
       Count(*) AS number_offices
FROM   noorders
GROUP  BY country
HAVING Count(*) > 0
ORDER  BY Count(*) DESC; 