SELECT p1.productcode AS p1,
       p2.productcode AS p2,
       Count(p1.ordernumber)
FROM   orderdetails p1
       JOIN orderdetails p2
         ON p1.ordernumber = p2.ordernumber
WHERE  p1.productcode < p2.productcode
GROUP  BY p1.productcode,
          p2.productcode
HAVING Count(p1.ordernumber) > 1
ORDER  BY p1.productcode,
          p2.productcode; 