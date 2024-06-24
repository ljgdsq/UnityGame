using System.Collections;
using System.Collections.Generic;
using System.Linq;
using UnityEngine;

public class Routing
{
    static Routing instance;
    public static Routing Instance
    {
        get
        {
            if (instance == null)
            {
                instance = new Routing();
            }
            return instance;
        }
    }

    RoutingObject[,] map;

    List<RoutingObject> open = new List<RoutingObject>();

    List<RoutingObject> close = new List<RoutingObject>();

    List<RoutingObject> route = new List<RoutingObject>();

    void Init(RoutingObject[,] mapArray)
    {
        open.Clear();
        close.Clear();
        route.Clear();
        map = mapArray;
    }

    public bool IsRouting(RoutingObject start,RoutingObject end, RoutingObject[,] mapArr)
    {
        Init(mapArr);
        Explore(start, end, start);
        return route.Count > 0;
    }
    void Explore(RoutingObject center,RoutingObject end,RoutingObject start)
    {
        close.Add(center);
        if (open.Contains(center))
        {
            open.Remove(center);
        }

        if (IsGetEnd(end))
        {
            RetureRoute(end, start);
        }
        else
        {
            // upper top
            if (center.y-1 >=0)
            {
                RoutingObject up = map[center.x, center.y - 1];
                GetMoveSumByDirection(up, center, end, Direction.up);
            }
        }

    }

    void GetMoveSumByDirection(RoutingObject center,RoutingObject start,RoutingObject end,Direction direction)
    {
        if (IsForward(center))
        {
            center.direction = direction;
            center.moveDistance = GetDistance(center, start);
            center.targetDistance = GetDistance(center, end);
            center.moveSum = center.moveDistance + center.targetDistance;
            open.Add(center);
        }
        else
        {
            Debug.LogError(center.name + " can not move");
        }
    }

    /// <summary>
    /// 这个点时候属于未来被考虑的点
    /// </summary>
    /// <param name="ro"></param>
    /// <returns></returns>
    bool IsForward(RoutingObject ro)
    {
        if (close.Contains(ro) || open.Contains(ro))
        {
            return false;
        }
        else
        {
            if (ro.isCanMove)
            {
                return true;
            }
            else
            {
                close.Add(ro);
                return false;
            }
        }
    }


    bool IsGetEnd(RoutingObject end)
    {
        return close.Contains(end);
    }

    int GetDistance(RoutingObject start,RoutingObject end)
    {
        return Mathf.Abs(start.x - end.x) + Mathf.Abs(start.y - end.y);
    }

    void RetureRoute(RoutingObject center,RoutingObject start)
    {
        route.Add(center);

        if (!route.Contains(start))
        {
            switch (center.direction)
            {
                case Direction.up:
                    RetureRoute(map[center.x, center.y + 1], start);
                    break;

            }
        }
        else
        {
            RouteSort(start);
        }


    }


    void RouteSort(RoutingObject start)
    {
        List<RoutingObject> list = new List<RoutingObject>(route);
        route.Clear();

        for (int i=list.Count-1;i>=0;i--)
        {
            if (list[i]!=start)
            {
                route.Add(list[i]);
            }
        }
    }

    public List<RoutingObject> GetRoute()
    {
        return route;
    }

}
