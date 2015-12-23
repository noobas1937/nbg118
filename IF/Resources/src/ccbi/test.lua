local test = {}

function test.create(ccbi)
    local memberTable = {}

    local function memberVarSetter(pMemberVariableName, node)
        if type(memberTable[pMemberVariableName]) == nil then
            cclog(ccbi .. ':this member var name duplicated -> ' .. pMemberVariableName)
        else
            memberTable[pMemberVariableName] = node
        end
    end

    local ccbNode = nbg.LuaCCBNode:create(ccbi, memberVarSetter)

    ccbNode['ccnode'] = tolua.cast(ccbNode, 'cc.Node')
    ccbNode['ccbi'] = ccbi
    
    -- members
    ccbNode['m_flower'] = memberTable['m_flower'] -- Sprite

    -- timelines
    ccbNode['Default Timeline'] = 'Default Timeline'

    -- export text 
    ccbNode['exportTexts'] = {}

    -- callbacks
    -- ccbNode:setLuaHandlerOnEnter(function ()
    --     ccbNode:setTouchEventListener(ccbNode['m_pGrpReturnHome'], -1, 0)
    -- end)
    -- 
    -- ccbNode:setLuaHandlerOnTouchClick(0, function (target, touch)
    --     if target == ccbNode['m_pGrpReturnHome'] then
    --         cclog('m_pGrpReturnHome')
    --     end
    -- end)
    -- 
    -- ccbNode:setLuaHandlerAnimationCallback(function (animationName)
    --     cclog(ccbNode['ccbi'] .. ':animation completed -> ' .. animationName)
    -- end)

    return ccbNode
end

return test