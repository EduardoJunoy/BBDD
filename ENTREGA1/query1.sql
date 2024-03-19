SELECT s1.customernumber,
       s1.customername,
       Sum(pm.amount)
FROM   (SELECT DISTINCT c.customernumber,
                        c.customername
        FROM   customers c
               JOIN orders o
                 ON c.customernumber = o.customernumber
               JOIN orderdetails od
                 ON o.ordernumber = od.ordernumber
               JOIN products p
                 ON od.productcode = p.productcode
        WHERE  p.productname = '1940 Ford Pickup Truck') AS s1
       JOIN payments pm
         ON s1.customernumber = pm.customernumber
GROUP  BY s1.customernumber,
          s1.customername
ORDER  BY Sum(pm.amount) DESC; 