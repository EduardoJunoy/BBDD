SELECT offices.officecode,
       Sum(orderdetails.quantityordered)
FROM   orderdetails
       NATURAL JOIN orders
       NATURAL JOIN customers
       JOIN employees
         ON ( customers.salesrepemployeenumber = employees.employeenumber )
       JOIN offices
         ON ( offices.officecode = employees.officecode )
GROUP  BY offices.officecode
ORDER  BY Sum(orderdetails.quantityordered) DESC
LIMIT  1; 