SELECT
        ii.YardReference,
        ii.Quantity,
        i.Name,
        FORMAT(ii.Length / 25.4, 'F0') + 'x' + FORMAT(ii.Width / 25.4, 'F0') AS Size,
        (((ii.Length / 25.4) - 15) * (ii.Width / 25.4) / 72) AS UsableArea,
        iigp.TrackingNumbers,
        CASE WHEN COALESCE(iigp.Allocations, '') = '' THEN 0 ELSE 1 END AS Allocated,
        iip.CEMark,
        iigp.Classifications
FROM dbo.ItemInventory ii
JOIN dbo.Item i
        ON i.ItemID = ii.ItemID
LEFT JOIN dbo.ItemInventoryGroupProperty iigp
        ON ii.ItemInventoryID = iigp.ItemInventoryID
LEFT JOIN dbo.ItemInventoryProperty iip
        ON ii.ItemInventoryPropertyID = iip.ItemInventoryPropertyID
WHERE ii.FacilityID = 3  -- In Berlin
        AND CHARINDEX('(', iigp.Allocations) = 0
        AND i.Name LIKE '%s"%%PLT%%'
        AND i.Name NOT LIKE '%%A588%%'
        AND i.Name NOT LIKE '%%GA%%'
        AND i.Name NOT LIKE '1/8"%%'
        AND i.Name NOT LIKE '3/16"%%'
        AND iigp.Classifications IN ('OK', 'Processed')
ORDER BY
        Allocated,
        i.Name,
        UsableArea,
        ii.Quantity
