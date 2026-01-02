#pragma once

#include <vector>
#include <memory>
#include <utility>
#include <cassert>
#include "state/state.h"
#include "draw/rgui.h"

namespace state {
    class StateStack final : public StateCommands {
        private:
            enum class OpType { Push, Pop, Replace, Clear };

            struct PendingOp {
                OpType type;
                std::unique_ptr<IState> state;
            };

            std::vector<std::unique_ptr<IState>> m_states;
            std::vector<PendingOp> m_pending;

            void doPush(std::unique_ptr<IState> s) {
                assert(s && "doPush: null state");
                s->OnEnter();
                m_states.push_back(std::move(s));
            }

            void doPop() {
                if (m_states.empty()) return;
                m_states.back()->OnExit();
                m_states.pop_back();
            }

            void doClear() {
                while (!m_states.empty()) {
                    m_states.back()->OnExit();
                    m_states.pop_back();
                }
            }

        public:
            // enqueu requests to change the state sstack
            void Push(std::unique_ptr<IState> s) override {
                m_pending.push_back(PendingOp{OpType::Push, std::move(s)});
            }

            void Pop() override {
                m_pending.push_back(PendingOp{OpType::Pop, nullptr});
            }

            void Replace(std::unique_ptr<IState> s) override {
                assert(s && "Replace: null state");
                m_pending.push_back(PendingOp{OpType::Replace, std::move(s)});
            }

            void Clear() override {
                m_pending.push_back(PendingOp{OpType::Clear, nullptr});
            }

            void HandleInput(StateContext& ctx) {
                if (m_states.empty()) return;
                // only top most state gets to act on input
                m_states.back()->HandleInput(ctx);
            }

            void Update(StateContext& ctx) {
                // update from top to bottom of stack until some state blocks us or we finish
                for (int i = (int)m_states.size() - 1; i >= 0; --i) {
                    m_states[i]->Update(ctx);
                    if (m_states[i]->BlocksUpdateBelow())
                        break;
                }
            }

            void Draw(StateContext& ctx) {
                if (m_states.empty()) return;

                // get lowest state that can be drawn
                int start = 0;
                for (int i = (int)m_states.size() - 1; i >= 0; --i) {
                    if (m_states[i]->BlocksDrawBelow()) {
                        start = i;
                        break;
                    }
                }

                // all guis below top level are to be disabled
                int top = (int)m_states.size() - 1;

                // draw from bottom to top so draw order is respected
                for (int i = start; i < (int)m_states.size(); ++i) {
                    if (i != top) rgui::LockGui();
                    m_states[i]->Draw(ctx);
                    if (i != top) rgui::UnlockGui();
                }
            }

            void ApplyPending() {
                for (auto& op : m_pending) {
                    switch (op.type) {
                        case OpType::Push:
                            doPush(std::move(op.state));
                            break;
                        case OpType::Pop:
                            doPop();
                            break;
                        case OpType::Replace:
                            doPop();
                            doPush(std::move(op.state));
                            break;
                        case OpType::Clear:
                            doClear();
                            break;
                    }
                }
                m_pending.clear();
            }

            bool Empty() const { return m_states.empty(); }
    };
}

